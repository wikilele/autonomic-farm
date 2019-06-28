#ifndef FARMWORKER_HPP
#define FARMWORKER_HPP

#include <iostream>
#include <src/masterthread/Scheduler.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
using namespace std;

#define EOS 0xffff

template <typename TIN, typename TOUT>
class MasterWorkerScheduler;

/**
 * This class manages the thread set up and the possibility
 * to freeze it
 * Everything else will be managed by the subclasses
 */
class AbstractWorker{ // FreezableWorker
    protected:
        mutex                  freeze_mutex; // LOCK
        condition_variable     freeze_condition;
        bool freezed = false;

        // the thread executing the farm logic fucntion
        thread worker_thread;

        void setFreezed(bool val){
            unique_lock<mutex> lock(this->freeze_mutex);
            freezed = val;
        }


        /**
         * This function checks if the worker is freezed and applies
         * the functions provided by the subclass
         * 
         * @param doIfFreezed - is called just before waiting to be defrosted
         * @param doAfterWaiting - is called after the defrost
         * @param doIfNotFreezed - is called if the thread is not freezed
         * 
         * CARE the three function must avoid possible deadlocks
         */
        void waitIfFreezed(function<void(void)> doIfFreezed,
                            function<void(void)> doAfterWaiting,
                            function<void(void)> doIfNotFreezed ){
                                
            unique_lock<mutex> lock(this->freeze_mutex);
            if (freezed == true){
                doIfFreezed();
                this->freeze_condition.wait(lock, [=]{ return this->freezed == false; });
                doAfterWaiting();
            } else {
                doIfNotFreezed();
            } 
        }

        /**
         * This function needs to be be implemented by a subclass
         * It will manage getting the task, calling the user defined 
         * function (i.e. lambda function) and it will return the result
        */ 
        virtual void main_task(){};

    public:      
        void freeze(){
            setFreezed(true);
        }

        void unfreeze(){ // the correct term would be defrost
            setFreezed(false);
            this->freeze_condition.notify_one();
        }

        void start(){
            worker_thread = thread(&AbstractWorker::main_task,this);
        }

        void join(){
            worker_thread.join();
        }

};

template <typename TIN, typename TOUT>
class FarmWorker : public AbstractWorker{
    protected:
        mutex                  task_mutex; // LOCK
        condition_variable     task_condition;
        // business logic code 
        function< TOUT* (TIN*)> user_task;
        // task to be computed
        TIN* task = NULL;

        MasterWorkerScheduler<TIN,TOUT>* scheduler;
        
        /**
         * After this functiontask is guaranteed to be non NULL
         */
        void waitForTask(){
            unique_lock<mutex> lock(this->task_mutex);
            if (task == NULL)
                this->task_condition.wait(lock, [=]{ return this->task != NULL; });
        }

        void returnResult(TOUT* result){
            // enqueuing just the result beacuse then the worker will wait to de defrosted
            auto doIfFreezed = [this,result](void){
                this->scheduler->enqueue(NULL,result);
            };
            // once defrosted the  worker is ready again 
            auto doAfterWaiting = [this](void){
                this->scheduler->enqueue(this,NULL);
            };
            // enquuing both the worker to get the next task and the result to collect it
            auto doIfNotFreezed = [this,result](void){
                this->scheduler->enqueue(this,result);
            };
            waitIfFreezed(doIfFreezed, doAfterWaiting, doIfNotFreezed);
        }


        void main_task(){
            bool eos = false;
            // ready to start
            this->scheduler->enqueue(this,NULL);
            while(!eos){
                waitForTask();
                // now task is guaranteed to be non null

                if(task == (TIN*)EOS){
                    eos = true;
                } else {
                    // applying business logic code given by the user
                    TOUT* result = user_task(task);
                    task = NULL;
                    returnResult(result);
                }
            }
            return;
        }

    public:
        FarmWorker( MasterWorkerScheduler<TIN,TOUT>* sched, function< TOUT* (TIN*)> ufunc): AbstractWorker() {
            scheduler = sched;
            user_task = ufunc;
        }

        void giveTaskANDnotify(TIN* t){
            {   
                unique_lock<mutex> lock(this->task_mutex);
                task = t;
            }
            this->task_condition.notify_one();
        }

};

#endif /* FARMWORKER_HPP */