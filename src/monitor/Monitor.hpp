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
        unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
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
            float time = chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

            float throughput = (task_collected / time)  ;

            int command = mstrategy->addWorker(throughput);
            
            if (command == ADDWORKER){
                if (workerpool->getTotalWorkers() < concurentThreadsSupported){
                cout << "ADDWORKER\n";
                workerpool->addWorker();
                }
            }else if (command == REMOVEWORKER){
                if (workerpool->getActualWorkers() > 1){
                    cout << "REMOVEWORKER\n";
                    workerpool->freezeWorker();
                }
            } 
            
            printf("EXPT %.1f - T %.2f - ANW %d - TOTNW %d \n", expected_throughput,
                                                        throughput,
                                                        workerpool->getActualWorkers(),
                                                        workerpool->getTotalWorkers());

        }
};


#endif /* MONITOR_HPP */