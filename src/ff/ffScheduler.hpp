#include <lib/fastflow/ff/ff.hpp>
#include <src/masterthread/Emitter.hpp>
#include <src/masterthread/Collector.hpp>
#include <src/ff/ffWorkerPool.hpp>
#include <src/ff/ffMonitor.hpp>
using namespace ff;


template <typename TIN, typename TOUT>
class ffScheduler: public ff_monode_t<TOUT,TIN> { // the master receives the results from the workers
    protected:
        unsigned int nw;
        IEmitter<TIN>* emitter;
        ICollector<TOUT>* collector;
        ffMonitor* monitor;
        ffWorkerPool<TIN,TOUT>* workerpool;
        int task_collected;
       
    public:
        ffScheduler(IEmitter<TIN>* emitter,  ICollector<TOUT>* collector, ffMonitor* monitor){
            this->emitter = emitter;
            this->collector = collector;
            this->monitor = monitor;
            task_collected = 0;
        }

        int svc_init() {
            nw = this->get_num_outchannels();  // at the beginning, this is the number of workers
            workerpool = new ffWorkerPool<TIN,TOUT>(nw);
            return 0;
        }

        TIN* svc(TOUT* t) {       
            int wid = this->get_channel_id();
            cout << "SCHED SVC\n";
            
            // the id of the Monitor channel is greater than the maximum id of the workers
            if ((size_t)wid == MONITORID) {  
                cout << "MONITOR";
                // the master just forwards the command to the WorkerPool
                Command_t *cmd = reinterpret_cast<Command_t*>(t);
                workerpool->addORfreezeWorker(cmd, this);
                return this->GO_ON;

            } else if ((size_t)wid < this->get_num_outchannels()) { // ack coming from the workers
                //printf("Emitter got %ld back from %d data.size=%ld, onthefly=%d\n", (long)t, wid, data.size(), onthefly);
                cout << "GETTING TASK BACK\n";
                TIN* newtask = emitter->getNextItem();
                task_collected ++;
                collector->pushResult(t);
                if (newtask != NULL)
                    this->ff_send_out_to(newtask, wid);
                else this->ff_send_out_to(this->EOS,wid);

                // TODO add the Monitor notify
                monitor->notify(task_collected);
                
                return this->GO_ON;
            } else{
                // first scheduling
                for (int i = 0; i < nw; i++){
                    TIN* newtask = emitter->getNextItem();
                    this->ff_send_out_to(newtask, i);
                }
                return this->GO_ON;
            }
        }

};
