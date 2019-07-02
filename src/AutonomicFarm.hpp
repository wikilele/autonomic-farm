

#include <src/masterthread/Scheduler.hpp>
#include <src/workers/WorkerPool.hpp>
#include <src/IAutonomicFarm.hpp>

template <typename TIN, typename TOUT>
class AutonomicFarm: public IAutonomicFarm<TIN,TOUT>{
    protected:
        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;
        MasterWorkerScheduler<TIN,TOUT>* scheduler;
        FarmWorkerPool<TIN,TOUT>* workerpool;
        Monitor* monitor;
    
    public:

        AutonomicFarm( IEmitter<TIN>* e,
                        ICollector<TOUT>* c,
                        MasterWorkerScheduler<TIN,TOUT>* sched,
                        FarmWorkerPool<TIN,TOUT>* wp,
                        Monitor* mon){
            
            emitter = e;
            collector = c;
            scheduler = sched;
            workerpool = wp;
            monitor = mon;
        }

        vector<TOUT*>* runAndGetResults(){
            scheduler->schedule();
            // relasing the threads correctly          
            int remaining_workers = workerpool->unfreezeRemainingWorkers();
            scheduler->sendLastEOS(remaining_workers);
            workerpool->joinPool();
            workerpool->terminate();
            return collector->returnResults();
        }

};