#ifndef MONITOR_HPP
#define MONITOR_HPP
#include <src/monitor/MonitorStrategy.hpp>
#include <src/workers/WorkerPool.hpp>

#include <iostream>
using namespace std;

class Monitor{
    protected:
        // task/milli seconds
        float expected_throughput;
        IMonitorStrategy* mstrategy;
        AbstractWorkerPool* workerpool;

        chrono::high_resolution_clock::time_point start;

    public:

        Monitor(IMonitorStrategy* ms, float et){
            mstrategy = ms;
            expected_throughput = et;
        }

        void initWorkerPool(AbstractWorkerPool* wp){
            workerpool = wp;
        }

        void init(){
            start = chrono::high_resolution_clock::now();
        }

        void notify(int task_collected){
            auto elapsed = chrono::high_resolution_clock::now() - start;
            long long time = chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

            float throughput = (task_collected / time)  ;
        

            int command = mstrategy->addWorker(expected_throughput, throughput);
            
            if (command == ADDWORKER){
                workerpool->addWorker();
            }else if (command == REMOVEWORKER){
                
                workerpool->freezeWorker();
            }
            
            
            printf("EXPT %.1f - T %.1f - ANW %d - TOTNW %d \n", expected_throughput,
                                                        throughput,
                                                        workerpool->getActualWorkers(),
                                                        workerpool->getTotalWorkers());

        }
};


#endif /* MONITOR_HPP */