#include <iostream>
#include <src/AutonomicFarmBuilder.hpp>
#include <lib/inputvectors/inputvectors.hpp>


int main(int argc, char * argv[]){

    // check input parameters
    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = atof(argv[2]);

    vector<int*>* vect = getInputVector();
   
    AutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(activewait)
                                            ->useDefaultMonitorStrategy(throughput)
                                            ->build();

    vector<int*>* results = afarm->runANDgetResults();
    
}