#include <iostream>
#include <src/AutonomicFarmBuilder.hpp>
#include <lib/inputvectors/inputvectors.hpp>

auto activewait = [](int *x) ->int*{
    auto start = std::chrono::high_resolution_clock::now();
    while (true)  {
      auto elapsed =  std::chrono::high_resolution_clock::now() - start;
      long long microseconds =  std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
      if (microseconds >= 1000* (*x)) break;
    }
    return x;
};


int main(int argc, char * argv[]){

    // check input parameters
    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = (float) atoi(argv[2]); //TODO check better this conversion when use tasks O(seconds)

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