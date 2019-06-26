#include <iostream>
#include <src/Scheduler.hpp>
#include <src/WorkerPool.hpp>

auto funz = [](int* x) -> int*{
    return new int(*x + 1);
};

int main(int argc, char * argv[]){
    // check input parameters
    vector<int> vect(7,42);
    IEmitter<int>* e = new DefaultEmitter<int>(&vect);
    ICollector<int>* c = new DefaultCollector<int>();
    Scheduler<int,int>* scheduler = new Scheduler<int,int>(e,c);
    FarmWorkerPool<int,int>* workerpool = new FarmWorkerPool<int,int>(scheduler,funz);
    workerpool->initPool(4);
    scheduler->schedule();

    vector<int*>* results = c->returnResults();

    for(auto i = (*results).begin(); i != (*results).end(); i++){
        cout << **i << endl;;
    }

}