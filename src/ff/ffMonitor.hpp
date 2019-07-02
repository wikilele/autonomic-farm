
#include <lib/fastflow/ff/ff.hpp>
#include <iostream>
using namespace ff;

// this is an id greater than all ids
const int MONITORID = MAX_NUM_THREADS+100;


#ifndef COMMAND_T
#define COMMAND_T
typedef enum { ADD, REMOVE } reconf_op_t;
struct Command_t {
    Command_t(int id, reconf_op_t op): id(id), op(op) {}
    int         id;
    reconf_op_t op;
};
#endif /* COMMAND_T */

class ffMonitor {
    protected:
        ff_buffernode  channel;
    public:
        ffMonitor(): channel(100, true, MONITORID) {}
        ff_buffernode * const getChannel() { return &channel;}
        
        Command_t* notify(int task_collected) {
            std::cout << task_collected << endl;
            
            Command_t *cmd1 = new Command_t(0, REMOVE);
            std::cout << "MA FA QUALCOSAAAAAAAAAAAAA\n";
            channel.ff_send_out(cmd1);
    /*
            Command_t *cmd2 = new Command_t(1, REMOVE);
            channel.ff_send_out(cmd2);



            Command_t *cmd3 = new Command_t(1, ADD);
            channel.ff_send_out(cmd3);

            Command_t *cmd4 = new Command_t(0, ADD);
            channel.ff_send_out(cmd4);


            channel.ff_send_out(EOS);*/

        }

        
};