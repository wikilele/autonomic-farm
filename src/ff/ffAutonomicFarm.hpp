
#include <lib/fastflow/ff/ff.hpp>
#include <src/IAutonomicFarm.hpp>
#include <src/ff/ffFarmWorker.hpp>
#include <src/ff/ffScheduler.hpp>
#include <src/monitor/DefaultMonitorStrategy.hpp>


template <typename TIN, typename TOUT>
class ffAutonomicFarm: public IAutonomicFarm<TIN,TOUT>{
    protected:
        ff_farm* farm;
        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;
        Monitor* monitor;

    public:
        ffAutonomicFarm(ff_farm* farm,
                        IEmitter<TIN>* emitter,
                        ICollector<TOUT>* collector,
                        Monitor* monitor){
            this->farm = farm;
            this->emitter = emitter;
            this->collector= collector;
            this->monitor = monitor;
        }

        vector<TOUT*>* runAndGetResults(){
            farm->run_then_freeze();
            farm->wait_freezing();

            return collector->returnResults();
        
        }

};