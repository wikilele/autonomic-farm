#ifndef MONITOR_HPP
#define MONITOR_HPP
#include <src/monitor/MonitorStrategy.hpp>
#include <src/workers/WorkerPool.hpp>

#include <iostream>
#include <chrono>
using namespace std;

class Monitor{
    protected:
        // task/milliseconds
        float expected_throughput;
        float previous_throughput;
        float overhead ;
        IMonitorStrategy* mstrategy;
        AbstractWorkerPool* workerpool;
        
        chrono::high_resolution_clock::time_point start; // when starting to count time  
        chrono::high_resolution_clock::time_point beginning; // just for output puposes 
        int old_taskcollected; // allows to refresh the actual throughput
    public:

        Monitor(IMonitorStrategy* ms, float et){
            mstrategy = ms;
            expected_throughput = et;
            previous_throughput = et;
            old_taskcollected = 0;
            overhead = 0;
        }

        void initWorkerPool(AbstractWorkerPool* wp){
            workerpool = wp;
        }

        void init(){
            start = chrono::high_resolution_clock::now();
            beginning = start;
            printf("time, throughput, nw, expected_throughput\n");
        }

        void notify(int task_collected, chrono::high_resolution_clock::time_point actual_now){
            float throughput;

            auto now = chrono::high_resolution_clock::now();
            auto elapsed = now - start;

            overhead +=  (long int)chrono::duration_cast<std::chrono::microseconds>(now - actual_now).count()/1000.0;
            
            float elapsed_milliseconds = (long int)chrono::duration_cast<std::chrono::microseconds>(elapsed).count()/1000.0;
            elapsed_milliseconds = elapsed_milliseconds - overhead;
            
            if (elapsed_milliseconds >= 0.500){
                throughput = (task_collected - old_taskcollected )/ elapsed_milliseconds;
                previous_throughput = throughput;
            } else {
                throughput = previous_throughput;
            }
                
            int command = mstrategy->addWorker(throughput);
                
            if (command & ADDWORKER){
                workerpool->addWorker();
            }else if (command & REMOVEWORKER){   
                workerpool->freezeWorker();
            } 
            
            float now_milliseconds = (long int)chrono::duration_cast<std::chrono::microseconds>(now - beginning).count()/1000.0;
            /*printf("EXPT %.1f - T %.2f - ANW %d - TOTNW %d \n", expected_throughput,
                                                            throughput,
                                                            workerpool->getActualWorkers(),
                                                            workerpool->getTotalWorkers());*/

            printf("%.2f, %.2f, %d, %.2f \n", now_milliseconds, throughput, workerpool->getActualWorkers(), expected_throughput);
            
            if (command & REFRESH_THROUGHPUT){
                start = chrono::high_resolution_clock::now(); 
                old_taskcollected = task_collected;
                overhead = 0;
            }
            

            return;
        }
};


#endif /* MONITOR_HPP */