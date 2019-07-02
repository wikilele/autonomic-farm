
#include <iostream>
#include <vector>
#include <lib/fastflow/ff/ff.hpp>
#include <lib/inputvectors/inputvectors.hpp>
#include <src/ff/ffFarmWorker.hpp>
#include <src/ff/ffScheduler.hpp>


using namespace ff;



int main(int argc, char* argv[]) {

    // check input parameters
    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = atof(argv[2]);

    vector<int*>* vect = getInputVector();
    

    ffMonitor* monitor = new ffMonitor();

    std::vector<ff_node* > workers;
    for(size_t i = 0; i < nw; i++){  
        workers.push_back(new ffFarmWorker<int,int>(activewait));
    }
    ff_farm farm(workers);
    farm.cleanup_workers();
    
    // registering the mranager channel as one extra input channel for the load balancer
    farm.getlb()->addManagerChannel(monitor->getChannel());
    
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
             
    //manager.run();
    
    farm.wait_freezing();
    
    farm.wait();
    
    //manager.wait();

    return 0;
}