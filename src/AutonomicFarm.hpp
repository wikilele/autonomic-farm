

#include <src/Scheduler.hpp>
#include <src/WorkerPool.hpp>

template <typename TIN, typename TOUT>
class AutonomicFarm{
    protected:
        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;
        MasterWorkerScheduler<TIN,TOUT>* scheduler;
        FarmWorkerPool<TIN,TOUT>* workerpool;
    
    public:

        AutonomicFarm( IEmitter<TIN>* e,
                        ICollector<TOUT>* c,
                        MasterWorkerScheduler<TIN,TOUT>* sched,
                        FarmWorkerPool<TIN,TOUT>* wp){
            
            emitter = e;
            collector = c;
            scheduler = sched;
            workerpool = wp;
        }

        vector<TOUT*>* runANDgetResults(){
            scheduler->schedule();
            return collector->returnResults();
        }

};