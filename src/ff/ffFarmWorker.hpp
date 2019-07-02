#include <lib/fastflow/ff/ff.hpp>
#include <iostream>
using namespace ff;


template <typename TIN, typename TOUT>
class ffFarmWorker: public ff_node_t<TIN,TOUT> {
    protected:
        // business logic code 
        function< TOUT* (TIN*)> user_task;
    public:
        ffFarmWorker(function< TOUT* (TIN*)> ufunc){
            user_task = ufunc;
        }

        TOUT* svc(TIN* task) {
            TOUT* result = user_task(task);
            
            // by sending the result the Master will know that this worker is ready           
            this->ff_send_out(result); 
            return this->GO_ON; 
        }

};