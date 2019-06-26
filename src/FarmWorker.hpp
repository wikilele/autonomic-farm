#ifndef FARMWORKER_HPP
#define FARMWORKER_HPP

#include <iostream>
#include <src/Scheduler.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
using namespace std;

/**
 * This class manages the thread set up and the possibility
 * to freeze it
 * Everything else will be managed by the subclasses
 */
class AbstractWorker{ // FreezableWorker
    protected:
        mutex                  freeze_mutex;
        condition_variable     freeze_condition;
        bool freezed = false;

        // the thread executing the business logic fucntion
        thread worker_thread;


        void setFreezed(bool val){
            unique_lock<mutex> lock(this->freeze_mutex);
            freezed = val; // TODO check this better dunno if locks can be good
        }

        /**
         * This function will be implemented by a subclass
         * It will manage getting the task, calling the user defined 
         * function (i.e. lambda function) 
         * and will return the result
        */ 
        virtual void main_task(){return;};

    public:      
        void freeze(){
            setFreezed(true);
        }

        // the correct term would be defrost
        void unfreeze(){
            setFreezed(false);
        }

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
        mutex                  task_mutex;
        condition_variable     task_condition;

        function< TOUT* (TIN*)> user_task;
        // task to be computed
        TIN* task = NULL;

        Scheduler<TIN,TOUT>* scheduler;

        
        void waitForTask(){
            unique_lock<mutex> lock(this->task_mutex);
            this->task_condition.wait(lock, [=]{ return this->task != NULL; });
        }
        
        void returnResult(TOUT* result){
            auto doIfFreezed = [this,result](void){
                this->scheduler->getQueue()->push(pair<FarmWorker<TIN,TOUT>*, TOUT*>(NULL,result));
            };
            auto doAfterWaiting = [this](void){
                // ready again
                this->scheduler->getQueue()->push(pair<FarmWorker<TIN,TOUT>*, TOUT*>(this,NULL));
            };
            auto doIfNotFreezed = [this,result](void){
                this->scheduler->getQueue()->push(pair<FarmWorker<TIN,TOUT>*, TOUT*>(this,result));
            };
            waitIfFreezed(doIfFreezed, doAfterWaiting, doIfNotFreezed);
        }


        void main_task(){
            // ready to start
            cout << "READY\n";
            this->scheduler->getQueue()->push(pair<FarmWorker<TIN,TOUT>*, TOUT*>(this,NULL));
            while(true){
                waitForTask();
                // now task is guaranteed to be non null

                // if TIN is EOS return TODO

                TOUT* result = compute_result(task);
                cout << "task computed\n";
                task = NULL;
                returnResult(result);
            }
        }

    public:
        FarmWorker(Scheduler<TIN,TOUT>* sched): AbstractWorker(){
            scheduler = sched;
        }


        FarmWorker( Scheduler<TIN,TOUT>* sched, function< TOUT* (TIN*)> ufunc): AbstractWorker() {
            scheduler = sched;
            user_task = ufunc;
        }

        // this function can be redefined by the framework user
        virtual TOUT* compute_result(TIN* input){
            return user_task(input); 
        }
        
        // ASSUME no concurrency
        void giveTaskANDnotify(TIN* t){
            task = t;
            this->task_condition.notify_one();
        }

};

#endif /* FARMWORKER_HPP */