#include <iostream>
#include <src/AutonomicFarmBuilder.hpp>
#include <lib/inputs.hpp>

int main(int argc, char * argv[]){

    // check input parameters
    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = (float) atoi(argv[2]); //TODO check better this conversion when use tasks O(seconds)

    vector<int*>* vect = getUpDownInputVector();
   
    AutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(activewait)
                                            ->useDefaultMonitorStrategy(throughput)
                                            ->build();

    vector<int*>* results = afarm->runANDgetResults();
    
}