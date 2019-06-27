#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <src/queue/Queue.hpp>
#include <src/Emitter.hpp>
#include <src/Collector.hpp>
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

    public:
        MasterWorkerScheduler(IEmitter<TIN>* emit, ICollector<TOUT>* collect){
            emitter = emit;
            collector = collect;
        }

        void enqueue(FarmWorker<TIN,TOUT>* fw, TOUT* result){
            queue.push(pair<FarmWorker<TIN,TOUT>*, TOUT*>(fw,result));
        }

        void schedule(){
            bool more_items = true;
            int total_task_number = 0;
            int task_collected = 0;

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
                         // TODO COMPUTE SERVICE TIME AND TAKE DECISION
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
};


#endif /* SCHEDULER_HPP */