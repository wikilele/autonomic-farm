#ifndef MONITOR_HPP
#define MONITOR_HPP
#include <src/monitor/MonitorStrategy.hpp>
#include <src/workers/WorkerPool.hpp>

#include <iostream>
using namespace std;

class Monitor{
    protected:
        // task/milliseconds
        float expected_throughput;
        IMonitorStrategy* mstrategy;
        AbstractWorkerPool* workerpool;
        
        chrono::high_resolution_clock::time_point start; // when starting to count time  
        int old_taskcollected; // allows to refresh the actual throughput
    public:

        Monitor(IMonitorStrategy* ms, float et){
            mstrategy = ms;
            expected_throughput = et;
            old_taskcollected = 0;
        }

        void initWorkerPool(AbstractWorkerPool* wp){
            workerpool = wp;
        }

        void init(){
            start = chrono::high_resolution_clock::now();
        }

        void notify(int task_collected){
            auto now = chrono::high_resolution_clock::now();
            auto elapsed = now - start;
            
            float elapsed_milliseconds = chrono::duration_cast<std::chrono::microseconds>(elapsed).count()/1000;
            
            if (elapsed_milliseconds != 0){
                // TODO somethign strange here
                float throughput = (task_collected - old_taskcollected )/ elapsed_milliseconds  ;
                
                int command = mstrategy->addWorker(throughput);
                
                if (command & ADDWORKER){
                    workerpool->addWorker();
                }else if (command & REMOVEWORKER){   
                    workerpool->freezeWorker();
                } 
                
                printf("EXPT %.1f - T %.2f - ANW %d - TOTNW %d \n", expected_throughput,
                                                            throughput,
                                                            workerpool->getActualWorkers(),
                                                            workerpool->getTotalWorkers());
                
                if (command & REFRESH_THROUGHPUT){
                    start = chrono::high_resolution_clock::now(); 
                    old_taskcollected = task_collected;
                }
            }

            return;
        }
};


#endif /* MONITOR_HPP */