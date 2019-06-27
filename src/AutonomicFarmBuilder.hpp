
#include <src/AutonomicFarm.hpp>

template <typename TIN, typename TOUT>
class AutonomicFarmBuilder{
    protected:
    int numberOfworkers;
    IEmitter<TIN>* emitter;
    ICollector<TOUT>* collector;
    Monitor* monitor;
    float expected_throughput = 0;
    IMonitorStrategy* mstrategy;
    function< TOUT* (TIN*)> user_function;
    public:
        AutonomicFarmBuilder(){}

        AutonomicFarmBuilder<TIN,TOUT>* useDefaultEmitter(vector<TIN*>* vect){
            emitter = new DefaultEmitter<TIN>(vect);
            return this;
        }

        AutonomicFarmBuilder<TIN,TOUT>* useDefaultCollector(){
            collector = new DefaultCollector<TOUT>();
            return this;
        }

        AutonomicFarmBuilder<TIN,TOUT>* setNumberOfWorkers(int nw){
            numberOfworkers = nw;
            return this;
        }

        AutonomicFarmBuilder<TIN,TOUT>* setWorkerFunction(function< TOUT* (TIN*)> func){
            user_function = func;
            return this;
        }

        AutonomicFarmBuilder<TIN,TOUT>* useDefaultMonitorStrategy(){
            mstrategy = new DefaultMonitorStrategy();
            return this;  
        }

        AutonomicFarmBuilder<TIN,TOUT>* useDumbMonitorStrategy(){
            mstrategy = new DumbMonitorStrategy();
            return this;  
        }

        AutonomicFarmBuilder<TIN,TOUT>* setExpectedThroughput(float et){
            expected_throughput = et;
            return this;
        }


        AutonomicFarm<TIN,TOUT>* build(){
            monitor = new Monitor(mstrategy, expected_throughput);
            MasterWorkerScheduler<TIN,TOUT>* scheduler = new MasterWorkerScheduler<TIN,TOUT>(emitter,collector,monitor);
            FarmWorkerPool<TIN,TOUT>* workerpool = new FarmWorkerPool<TIN,TOUT>(scheduler,user_function);

            monitor->initWorkerPool(workerpool);
            workerpool->initPool(numberOfworkers);
            AutonomicFarm<TIN,TOUT>* afarm = new AutonomicFarm<TIN,TOUT>(emitter,
                                                    collector,
                                                    scheduler,
                                                    workerpool,
                                                    monitor);
            
            return afarm;
        }



};