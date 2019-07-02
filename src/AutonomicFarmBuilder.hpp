
#include <src/AutonomicFarm.hpp>
#include <src/monitor/DefaultMonitorStrategy.hpp>

#ifdef USE_FASTFLOW
#include <src/ff/ffAutonomicFarm.hpp>
#endif /* USE_FASTFLOW */

template <typename TIN, typename TOUT>
class AutonomicFarmBuilder{
    protected:
    int numberOfworkers;
    int inputsize = 0;
    IEmitter<TIN>* emitter;
    ICollector<TOUT>* collector;
    Monitor* monitor;
    float expected_throughput;
    IMonitorStrategy* mstrategy;
    function< TOUT* (TIN*)> user_function;
    bool use_fastflow = false;

    IAutonomicFarm<TIN,TOUT>* nativeCthreads_build(){
            monitor = new Monitor(mstrategy, this->expected_throughput);
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
#ifdef USE_FASTFLOW
    IAutonomicFarm<TIN,TOUT>* fastflow_build(){
        monitor = new Monitor(mstrategy, this->expected_throughput);
        
        vector<ff::ff_node* > workers ;
        for(size_t i = 0; i < numberOfworkers; i++){  
            workers.push_back(new ffFarmWorker<TIN,TOUT>(user_function));
        }
        ff::ff_farm* farm = new ff::ff_farm(workers);
        farm->cleanup_workers();

        ffScheduler<TIN,TOUT>* scheduler = new ffScheduler<TIN,TOUT>(emitter,collector,monitor);

        farm->remove_collector();
        farm->add_emitter(scheduler); 
        farm->wrap_around();

        ffAutonomicFarm<TIN,TOUT>* afarm = new ffAutonomicFarm<TIN,TOUT>(farm,emitter,collector,monitor);

        return afarm;
    }
#endif /* USE_FASTFLOW */

    public:
        AutonomicFarmBuilder(){}

        AutonomicFarmBuilder<TIN,TOUT>* useDefaultEmitter(vector<TIN*>* vect){
            inputsize = vect->size();
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

        AutonomicFarmBuilder<TIN,TOUT>* useDefaultMonitorStrategy(float  expected_throughput){
            this->expected_throughput = expected_throughput;
            
            if (inputsize <= 0){
                cout << "!!! The Emitter function should be called before this one !!!" << endl;
                cout << "Exiting ...." << endl;
                exit(-1);
            }

            mstrategy = new DefaultMonitorStrategy( expected_throughput, inputsize);
            return this;  
        }
#ifdef USE_FASTFLOW
        AutonomicFarmBuilder<TIN,TOUT>* useFastFlow(){
            use_fastflow = true;
            return this;
        }
#endif /* USE_FASTFLOW */

        IAutonomicFarm<TIN,TOUT>* build(){
            IAutonomicFarm<TIN,TOUT>* afarm;
            if (!use_fastflow)
                afarm = nativeCthreads_build();            
#ifdef USE_FASTFLOW           
            else afarm = fastflow_build(); 
#endif /* USE_FASTFLOW */
            return afarm;
        }



};