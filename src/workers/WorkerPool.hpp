#ifndef WORKERPOOL_HPP
#define WORKERPOOL_HPP

#include <src/workers/FarmWorker.hpp>
#include <vector>
#include <thread>
using namespace std;

class AbstractWorkerPool{
    protected:
        vector<AbstractWorker*> farm_workers;
        // number of non freezed workers, always <= farm_workers size
        int actual_workers; 
        unsigned concurentThreadsSupported = std::thread::hardware_concurrency();


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

        void joinPool(){
            for(int i = 0; i < farm_workers.size(); i++){
                (farm_workers[i])->join();
            }
        }


        void addWorker(){          
            if(actual_workers < farm_workers.size()){
                // wake up a worker
                // start counting from 0
                farm_workers.at(actual_workers )->unfreeze();
                actual_workers ++;
                //cout << "ADDWORKER\n";
            } else if (this->getTotalWorkers() < concurentThreadsSupported){
                spawnANDstartWorker();
                actual_workers ++;

                //cout << "ADDWORKER\n";
            }     
        }

        void freezeWorker(){
            if (actual_workers > 1){
                //cout << "REMOVEWORKER\n";
                farm_workers.at(actual_workers - 1)->freeze();
                actual_workers --;
            }
        }

        /**
         * This function will be called at the end to release the workers properly
         * 
         * @return - the number of worker defrosted
         */
        int unfreezeRemainingWorkers(){
            int unfreezed_workers = 0;;
            for(int i = 0; i < farm_workers.size(); i++){
                if (farm_workers[i]->isFreezed()){
                    farm_workers[i]->unfreeze();
                    unfreezed_workers ++;
                }
            }

            return unfreezed_workers;
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