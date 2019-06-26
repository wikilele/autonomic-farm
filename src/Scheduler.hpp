#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <src/queue/Queue.hpp>
#include <src/Emitter.hpp>
#include <src/Collector.hpp>
#include <thread>
using namespace std;

template <typename TIN, typename TOUT>
class FarmWorker;

template <typename TIN, typename TOUT>
class Scheduler{
    protected:
        Queue<pair<FarmWorker<TIN,TOUT>*, TOUT*>> queue;

        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;

    public:
        Scheduler(IEmitter<TIN>* emit, ICollector<TOUT>* collect){
            emitter = emit;
            collector = collect;
        }

        Queue<pair<FarmWorker<TIN,TOUT>*, TOUT*>>* getQueue(){
            return &queue;
        }

        void schedule(){
            int total_task_number = 0;
            int task_collected = 0;
            TIN* task;
            while( (task = emitter->getNextItem()) != NULL) {
                total_task_number ++;
                pair<FarmWorker<TIN,TOUT>*, TOUT*> popped_pair = queue.pop();
                // unpacking the pair
                FarmWorker<TIN,TOUT>* readyworker = popped_pair.first;
                TOUT* result = popped_pair.second;
                if (result != NULL){
                    task_collected ++;
                    collector->pushResult(result);
                }

                readyworker->giveTaskANDnotify(task);
                
            }

            // waiting the last workers
            while(task_collected < total_task_number){
                pair<FarmWorker<TIN,TOUT>*, TOUT*> popped_pair = queue.pop();
                TOUT* result = popped_pair.second;
                if (result != NULL){
                    task_collected ++;
                    collector->pushResult(result);
                }
            }
        }
};


#endif /* SCHEDULER_HPP */