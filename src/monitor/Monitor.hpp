#ifndef MONITOR_HPP
#define MONITOR_HPP
#include <src/monitor/MonitorStrategy.hpp>
#include <src/workers/IAddRemoveWorker.hpp>

#include <iostream>
#include <chrono>
using namespace std;

class Monitor{
    protected:
        // task/milliseconds
        float expected_throughput;
        float previous_throughput;
        IMonitorStrategy* mstrategy;
        IAddRemoveWorker* workerpool;
        
        chrono::high_resolution_clock::time_point start; // when starting to count time  
        chrono::high_resolution_clock::time_point beginning; // just for output puposes 
        int old_taskcollected; // allows to refresh the actual throughput
    public:

        Monitor(IMonitorStrategy* ms, float et){
            mstrategy = ms;
            expected_throughput = et;
            previous_throughput = et;
            old_taskcollected = 0;
        }

        void initWorkerPool(IAddRemoveWorker* wp){
            workerpool = wp;
        }

        void init(){
            start = chrono::high_resolution_clock::now();
            beginning = start;
            // output to be redireted to a .csv file
            printf("time, throughput, nw\n");
        }

        void notify(int task_collected){
            float throughput;

            auto now = chrono::high_resolution_clock::now();
            auto elapsed = now - start;

            float elapsed_milliseconds = (long int)chrono::duration_cast<std::chrono::microseconds>(elapsed).count()/1000.0;
#ifdef DEBUG
            printf("task collected %d - elapsed_milliec %f\n",task_collected - old_taskcollected, elapsed_milliseconds);
#endif /* DEBUG */
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
                workerpool->removeWorker();
            } 
            
            float now_milliseconds = (long int)chrono::duration_cast<std::chrono::microseconds>(now - beginning).count()/1000.0;
#ifdef DEBUG
            printf("EXPT %.1f - T %.2f - ANW %d - TOTNW %d \n", expected_throughput,
                                                            throughput,
                                                            workerpool->getActualWorkers(),
                                                            workerpool->getTotalWorkers());
#else
            //output to be redireted to a .csv file
            printf("%.2f, %.2f, %d\n", now_milliseconds, throughput, workerpool->getActualWorkers());
#endif /* DEBUG */
            if (command & REFRESH_THROUGHPUT){
                start = chrono::high_resolution_clock::now(); 
                old_taskcollected = task_collected;
            }
            

            return;
        }
};


#endif /* MONITOR_HPP */