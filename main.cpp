#include <iostream>
#include <src/AutonomicFarmBuilder.hpp>
#include <assert.h>

auto funz = [](int* x) -> int*{
    return new int(*x + 1);
};

int main(int argc, char * argv[]){
    // check input parameters
    vector<int*> vect;
    for(int i = 0; i < 7; i ++ ){
        vect.push_back(new int(i));
    } 
    int nw = 4;
    AutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(&vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(funz)
                                            ->build();

    vector<int*>* results = afarm->runANDgetResults();
    

    assert(results->size() == 7);
    
    /* for(auto i = (*results).begin(); i != (*results).end(); i++){
        cout << **i << endl;;
    } */

}