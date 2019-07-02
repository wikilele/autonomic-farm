
#include <iostream>
#include <vector>
#include <lib/inputvectors/inputvectors.hpp>
#include <src/AutonomicFarmBuilder.hpp>
#define USE_FASTFLOW

int main(int argc, char* argv[]) {

    // check input parameters
    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = atof(argv[2]);

    vector<int*>* vect = getInputVector();
    
    /*IMonitorStrategy* mstrategy = new DefaultMonitorStrategy(throughput, vect->size());
    Monitor* monitor = new Monitor(mstrategy,throughput);

    std::vector<ff_node* > workers;
    for(size_t i = 0; i < nw; i++){  
        workers.push_back(new ffFarmWorker<int,int>(activewait));
    }
    ff_farm farm(workers);
    farm.cleanup_workers();

    
    DefaultEmitter<int> * emitter = new DefaultEmitter<int>(vect);
    DefaultCollector<int>* collector = new DefaultCollector<int>();
    ffScheduler<int,int> scheduler(emitter,collector,monitor);

    farm.remove_collector();
    farm.add_emitter(&scheduler); 
    farm.wrap_around();
    
    
    if (farm.run_then_freeze()<0) {
        error("running farm\n");
        return -1;
    }
             
    farm.wait_freezing();*/


    IAutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(activewait)
                                            ->useDefaultMonitorStrategy(throughput)
                                            ->useFastFlow()
                                            ->build();

    vector<int*>* results = afarm->runAndGetResults();
    return 0;
}