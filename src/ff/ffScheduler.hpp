#include <lib/fastflow/ff/ff.hpp>
#include <src/masterthread/Emitter.hpp>
#include <src/masterthread/Collector.hpp>
#include <src/ff/ffWorkerPool.hpp>
#include <src/monitor/Monitor.hpp>
using namespace ff;


template <typename TIN, typename TOUT>
class ffScheduler: public ff_monode_t<TOUT,TIN> { // the master receives the results from the workers
    protected:
        unsigned int nw;
        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;
        Monitor* monitor;
        ffWorkerPool<TIN,TOUT>* workerpool;
        int task_collected;
        int task_emitted;
       
    public:
        ffScheduler(IEmitter<TIN>* emitter,  ICollector<TOUT>* collector, Monitor* monitor){
            this->emitter = emitter;
            this->collector = collector;
            this->monitor = monitor;
            task_collected = 0;
            task_emitted = 0;
        }

        int svc_init() {
              // at the beginning, this is the number of workers
            nw = this->get_num_outchannels();
            workerpool = new ffWorkerPool<TIN,TOUT>(this);          
            monitor->initWorkerPool(workerpool);
            
            return 0;
        }

        TIN* svc(TOUT* t) {       
            int wid = this->get_channel_id();
            
            // result and ack coming from a worker
            if ((size_t)wid < this->get_num_outchannels()) { 
                TIN* newtask = emitter->getNextItem();
                task_collected ++;
                collector->pushResult(t);
                //printf("task emitted %d - task collected %d\n",task_emitted,task_collected);
                if (newtask != NULL){                 
                    task_emitted ++;
                    this->ff_send_out_to(newtask, wid);
                    monitor->notify(task_collected);
                } else {
                    // managing the termination
                    workerpool->unfreezeRemainingWorkers();
       
                    if (task_collected >= task_emitted){                    
                        this->broadcast_task(this->EOS_NOFREEZE);
                        return this->EOS;
                    }
                }              
                
                return this->GO_ON;
            } else{
                // first scheduling
                monitor->init();
                for (int i = 0; i < nw; i++){
                    // ASSUME there are at leat nw tasks in the vector
                    TIN* newtask = emitter->getNextItem(); 
                    task_emitted ++;
                    this->ff_send_out_to(newtask, i);
                }
                return this->GO_ON;
            }
        }

};
