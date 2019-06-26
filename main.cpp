#include <iostream>
#include <src/AutonomicFarmBuilder.hpp>


auto funz = [](int* x) -> int*{
    return new int(*x + 1);
};

int main(int argc, char * argv[]){
    // check input parameters
    vector<int> vect;
    for(int i = 0; i < 7; i ++ ){
        vect.push_back(i);
    } 
    int nw = 4;
    AutonomicFarm<int,int>* afarm =  (new AutonomicFarmBuilder<int,int>())
                                            ->useDefaultEmitter(&vect)
                                            ->useDefaultCollector()
                                            ->setNumberOfWorkers(nw)
                                            ->setWorkerFunction(funz)
                                            ->build();

    vector<int*>* results = afarm->runANDgetResults();
    
    for(auto i = (*results).begin(); i != (*results).end(); i++){
        cout << **i << endl;;
    }

}