#include <iostream>
#include <src/AutonomicFarmBuilder.hpp>
#include <assert.h>

auto funz = [](int* x) -> int*{
    return new int(*x + 1);
};


auto activewait = [](int *x) ->int*{
    auto start = chrono::high_resolution_clock::now();
    while (true)  {
      auto elapsed = chrono::high_resolution_clock::now() - start;
      long long microseconds = chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

      if (microseconds >= 1000* (*x)) break;
    }
    return x;
};

int main(int argc, char * argv[]){

    if(argc < 3) {
        cout << "Usage is: [nw] [throughput] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    float throughput = (float) atoi(argv[2]); //TODO check better this conversion when use tasks O(seconds)

    // check input parameters
    vector<int*> vect;
    for(int i = 0; i < 10; i ++ ){
        vect.push_back(new int(1));
    } 

    nw = 4;
    throughput = 4.0;
    
    for(int i = 0; i < 10; i ++ ){
        vect.push_back(new int(1));
    }

    for(int i = 0; i < 10; i ++ ){
        vect.push_back(new int(1));
    }
   
    AutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(&vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(activewait)
                                            ->setExpectedThroughput(throughput)
                                            ->useDumbMonitorStrategy()
                                            ->build();

    vector<int*>* results = afarm->runANDgetResults();
    

    //assert(results->size() == 7);
    /*
    for(auto i = (*results).begin(); i != (*results).end(); i++){
        cout << **i << endl;;
    }*/ 

}