
#include <iostream>
#include <vector>
#include <lib/inputvectors/inputvectors.hpp>
#include <src/AutonomicFarmBuilder.hpp>


int main(int argc, char* argv[]) {

    // check input parameters
    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = atof(argv[2]);

    vector<int*>* vect = getInputVector();
    
    IAutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(activewait)
                                            ->useGeneralMonitorStrategy(throughput)
                                            ->useFastFlow()
                                            ->build();

    vector<int*>* results = afarm->runAndGetResults();
    return 0;
}