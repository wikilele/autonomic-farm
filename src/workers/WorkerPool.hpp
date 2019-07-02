#ifndef WORKERPOOL_HPP
#define WORKERPOOL_HPP

#include <src/workers/FarmWorker.hpp>
#include <src/workers/IAddRemoveWorker.hpp>

#include <vector>
#include <thread>
using namespace std;

#define ADDWORKER_WP 2
#define REMOVEWORKER_WP 1
#define TERMINATE_WP 0 

class AbstractWorkerPool: public IAddRemoveWorker{
    protected:
        vector<AbstractWorker*> farm_workers;
        // number of non freezed workers, always <= farm_workers size
        int actual_workers; 
        unsigned concurentThreadsSupported = std::thread::hardware_concurrency();

        Queue<int> command_q;
        /**
         * This thread fetches the ADD or REMOVE commands from the above queue
         * to add or freeze threads. It is extremely important when a brand new thread is needed.
         * This thread will take charge of the thread spawning overhead so that the
         * Scheduler is free to collect results.
         */
        thread command_thread;

        void spawnANDstartWorker(){
            AbstractWorker* abworker = spawnWorker();
            farm_workers.push_back(abworker);
            abworker->start(); 
        }

        // cmd must be ADDWORKER_WP or REMOVEWORKER_WP or TERMINATE_WP
        void pushCommand(int cmd){
            command_q.push(cmd);
        }

        void execute_command(){
            bool end = false;
            while(!end){
                int cmd = command_q.pop();
                if (cmd == ADDWORKER_WP) __addWorker();
                else if (cmd == REMOVEWORKER_WP) __freezeWorker();
                else end = true;
            }
        }

        void __addWorker(){    
     
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

        void __freezeWorker(){
            if (actual_workers > 1){
                //cout << "REMOVEWORKER\n";
                farm_workers.at(actual_workers - 1)->freeze();
                actual_workers --;
            }
        }

    public:
        // nw - parallelism degree
        void initPool(int nw){
            for(int i= 0; i < nw; i ++){
                spawnANDstartWorker();
            }
            actual_workers = nw;
            command_thread = thread(&AbstractWorkerPool::execute_command,this);
        }

        void joinPool(){
            for(int i = 0; i < farm_workers.size(); i++){
                (farm_workers[i])->join();
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

        void terminate(){
            this->pushCommand(TERMINATE_WP);
        }

        /** IAddRemoveWorker methods */
        void addWorker(){
            this->pushCommand(ADDWORKER_WP);
        }

        void removeWorker(){
            this->pushCommand(REMOVEWORKER_WP);
        }

        int getActualWorkers(){ return actual_workers; }
        int getTotalWorkers(){ return farm_workers.size();}
        /*****************************/
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