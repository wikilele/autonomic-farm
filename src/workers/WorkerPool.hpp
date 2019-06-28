#ifndef WORKERPOOL_HPP
#define WORKERPOOL_HPP

#include <src/workers/FarmWorker.hpp>
#include <vector>
#include <thread>
using namespace std;

// TODO join workers

class AbstractWorkerPool{
    protected:
        vector<AbstractWorker*> farm_workers;
        // number of non freezed workers, always <= farm_workers size
        int actual_workers; 

        void spawnANDstartWorker(){
            AbstractWorker* abworker = spawnWorker();
            farm_workers.push_back(abworker);
            abworker->start(); 
        }

    public:
        // nw - parallelism degree
        void initPool(int nw){
            for(int i= 0; i < nw; i ++){
                spawnANDstartWorker();
            }
            actual_workers = nw;
        }


        void addWorker(){
            if(actual_workers < farm_workers.size()){
                // wake up a worker
                // start counting from 0
                farm_workers.at(actual_workers )->unfreeze();
            } else{
                spawnANDstartWorker();
            }
            actual_workers ++;
        }

        void freezeWorker(){
            farm_workers.at(actual_workers - 1)->freeze();
            actual_workers --;
        }

        int getActualWorkers(){ return actual_workers; }
        int getTotalWorkers(){ return farm_workers.size();}

        virtual AbstractWorker* spawnWorker() = 0;
};



template <typename TIN, typename TOUT>
class FarmWorkerPool: public AbstractWorkerPool{
    protected:
        MasterWorkerScheduler<TIN,TOUT> * scheduler;
        function< TOUT* (TIN*)> user_task;
    public:
        FarmWorkerPool(MasterWorkerScheduler<TIN,TOUT> * sched, function< TOUT* (TIN*)> ufunc){
            scheduler = sched;
            user_task = ufunc;
        }

        virtual AbstractWorker* spawnWorker(){
            return new FarmWorker<TIN,TOUT>(scheduler, user_task);
        }
};


#endif /* WORKERPOOL_HPP */