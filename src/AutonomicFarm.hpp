

#include <src/Scheduler.hpp>
#include <src/WorkerPool.hpp>

template <typename TIN, typename TOUT>
class AutonomicFarm{
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

        vector<TOUT*>* runANDgetResults(){
            scheduler->schedule();
            return collector->returnResults();
        }

};