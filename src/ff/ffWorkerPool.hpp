#include <lib/fastflow/ff/ff.hpp>
#include <src/workers/IAddRemoveWorker.hpp>
using namespace ff;

template <typename TIN, typename TOUT>
class ffScheduler;

/**
 * Since I use a reference to the Master<TIN,TOUT> inside I need to declare this class
 * with the template :(
 */
template <typename TIN, typename TOUT>
class ffWorkerPool: public IAddRemoveWorker {
    protected:
        int  total_nw;
        int actual_nw;

         ffScheduler<TIN,TOUT>* scheduler;

    public: 
        ffWorkerPool( ffScheduler<TIN,TOUT>* scheduler){
            this->scheduler = scheduler;
            total_nw = actual_nw = scheduler->get_num_outchannels();
        }
        
        /** IAddRemoveWorker methods */
        void addWorker(){
            if (actual_nw < total_nw){
                cout << "ADDWORKER\n";
                actual_nw ++;
                // worker id starts from 0
                scheduler->getlb()->thaw(actual_nw - 1 , true);
            }
        }

        void removeWorker(){
            if(actual_nw > 1){ 
                cout << "REMOVEWORKER\n";
                actual_nw --;
                scheduler->ff_send_out_to(scheduler->GO_OUT, actual_nw);
            }
        }

        int getActualWorkers(){ return actual_nw; }
        int getTotalWorkers(){ return total_nw;}
        /*****************************/

        void unfreezeRemainingWorkers(){
            for(actual_nw; actual_nw <= total_nw; actual_nw++){
                scheduler->getlb()->thaw(actual_nw -1 , true);
            }
        }
};