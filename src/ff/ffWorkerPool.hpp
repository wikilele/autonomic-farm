#include <lib/fastflow/ff/ff.hpp>
using namespace ff;

template <typename TIN, typename TOUT>
class ffScheduler;

#ifndef COMMAND_T
#define COMMAND_T
typedef enum { ADD, REMOVE } reconf_op_t;
struct Command_t {
    Command_t(int id, reconf_op_t op): id(id), op(op) {}
    int         id;
    reconf_op_t op;
};
#endif /* COMMAND_T */
/**
 * Since I use a reference to the Master<TIN,TOUT> inside I need to declare this class
 * with the template :(
 */
template <typename TIN, typename TOUT>
class ffWorkerPool{
    protected:
        unsigned int  nw;
        unsigned int readyworkers_num;
        unsigned int frozenworkers_num;

        std::vector<bool>  isready_worker;         // which workers are ready
        std::vector<bool>  isfreezed_worker;      // which workers are sleeping
        std::vector<long*> data;          // local storage

    public: 
        ffWorkerPool(unsigned int nw){
            this->nw = nw;  
            isready_worker.resize(nw); 
            isfreezed_worker.resize(nw);
            for(size_t i = 0; i < isready_worker.size(); i++) {
                isready_worker[i]    = true;
                isfreezed_worker[i] = false;
            }
            readyworkers_num = isready_worker.size();
            frozenworkers_num = 0;
        }
/*
        int selectReadyWorker() {
            for (unsigned i=last+1;i<ready.size();++i) {
                if (ready[i]) {
                    last = i;
                    return i;
                }
            }
            for (unsigned i=0;i<=last;++i) {
                if (ready[i]) {
                    last = i;
                    return i;
                }
            }
            return -1;
        }*/

        void addORfreezeWorker(Command_t* cmd,  ffScheduler<TIN,TOUT>* master){
            // unpacking the command
            int workerid = cmd->id;
            reconf_op_t operation = cmd-> op;
            if (operation == ADD){
                master->getlb()->thaw(workerid, true);
                //assert(isfreezed_worker[workerid]);
                isfreezed_worker[workerid] = false;
                frozenworkers_num--;
            } else if (operation == REMOVE){
                master->ff_send_out_to(master->GO_OUT, workerid);
                //assert(!isfreezed_worker[workerid]);
                isfreezed_worker[workerid] = true;
                frozenworkers_num++;
            }

            delete cmd;        
        }

};