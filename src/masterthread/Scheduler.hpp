#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <src/queue/Queue.hpp>
#include <src/masterthread/Emitter.hpp>
#include <src/masterthread/Collector.hpp>
#include <src/monitor/Monitor.hpp>
#include <thread>
using namespace std;

#define EOS 0xffff

template <typename TIN, typename TOUT>
class FarmWorker;


template <typename TIN, typename TOUT>
class MasterWorkerScheduler{
    protected:
        Queue<pair<FarmWorker<TIN,TOUT>*, TOUT*>> queue;

        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;
        Monitor* monitor;

        /**
         * Since the scheduler has an importat role it is sticked to gain in
         * performances 
         */
        void stickOnCore(){
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(sched_getcpu(), &cpuset); 

            pthread_t current_thread = pthread_self(); 
            pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset); 
        }

    public:
        MasterWorkerScheduler(IEmitter<TIN>* emit,
                                ICollector<TOUT>* collect,
                                Monitor* mon){
            emitter = emit;
            collector = collect;
            monitor = mon;
        }

        void enqueue(FarmWorker<TIN,TOUT>* fw, TOUT* result){
            queue.push(pair<FarmWorker<TIN,TOUT>*, TOUT*>(fw,result));
        }

        void schedule(){
            stickOnCore();

            bool more_items = true;
            int total_task_number = 0;
            int task_collected = 0;

            monitor->init();   
                     
            while(more_items) {                
                // popping all the waiting workers allows to lock the queue just one time
                // and process more element, istead of lock - process - lock - process ....
                vector<pair<FarmWorker<TIN,TOUT>*, TOUT*>> poppedpairs_vector = queue.popAll();

                for (auto iter = poppedpairs_vector.begin(); iter != poppedpairs_vector.end(); iter ++){
                    // unpacking the pair
                    FarmWorker<TIN,TOUT>* readyworker = (*iter).first;
                    TOUT* result = (*iter).second;

                    if (result != NULL){
                        task_collected ++;
                        collector->pushResult(result);
                    }
                   
                    if (readyworker != NULL){
                        TIN* task;
                        if ( (task = emitter->getNextItem()) != NULL){
                            total_task_number ++;
                            readyworker->giveTaskANDnotify(task);
                        } else {
                            more_items = false;
                            readyworker->giveTaskANDnotify((TIN*) EOS);
                        }
                    }
                }

                //Copute service time and take a decision
                if (task_collected > 0)
                    monitor->notify(task_collected); 
            }
      
            // waiting the last workers
            while(task_collected < total_task_number){
                pair<FarmWorker<TIN,TOUT>*, TOUT*> popped_pair = queue.pop();
                
                // unpacking the pair
                FarmWorker<TIN,TOUT>* readyworker = popped_pair.first;
                TOUT* result = popped_pair.second;
                
                if (result != NULL){
                    task_collected ++;    
                    collector->pushResult(result);
                }
                
                if (readyworker != NULL){
                    readyworker->giveTaskANDnotify((TIN*) EOS);
                }
            }

        }


        void sendLastEOS(int remaining_workers){
            while (remaining_workers > 0){
                pair<FarmWorker<TIN,TOUT>*, TOUT*> popped_pair = queue.pop();

                FarmWorker<TIN,TOUT>* readyworker = popped_pair.first;
                if (readyworker != NULL){
                    readyworker->giveTaskANDnotify((TIN*) EOS);
                    remaining_workers --;
                }
            }            
        }
};


#endif /* SCHEDULER_HPP */