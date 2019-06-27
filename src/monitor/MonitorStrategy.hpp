#ifndef MONITORSTRATEGY_HPP
#define MONITORSTRATEGY_HPP

#include <stdlib.h>     /* srand, rand */
#include <time.h>

#define ADDWORKER 1
#define REMOVEWORKER -1
#define DONOTHING 0

// TODO consider to define a threshold
#define THRESHOLD 1


class IMonitorStrategy{
    public:
        virtual int addWorker(float expected_throughput, float actual_throughput) = 0;
};

class DumbMonitorStrategy: public IMonitorStrategy{
    public:
        virtual int addWorker(float expected_throughput, float actual_throughput){
            /*if (expected_throughput + THRESHOLD >=  actual_throughput &&
                        actual_throughput >= expected_throughput - THRESHOLD)
                return DONOTHING;

            if (expected_throughput >  actual_throughput)
                return ADDWORKER;
            else if (expected_throughput < actual_throughput)
                return REMOVEWORKER;
            else return DONOTHING;*/
            /*
            srand (time(NULL));
            int random = rand() % 100 + 1 ;

            if (random <= 33) return ADDWORKER;
            else if (random > 33 && random <=66) return REMOVEWORKER;
            else return DONOTHING;*/

            static int driver = -1;
            driver++;
            if (driver %3 == 0)
                return DONOTHING;
            else if(driver%3 == 1){
                return REMOVEWORKER;
            } else return ADDWORKER;

        }
};



class DefaultMonitorStrategy: public IMonitorStrategy{
    public:
        virtual int addWorker(float expected_throughput, float actual_throughput){
            return ADDWORKER; // TODO define this
        }
};




#endif /* MONITORSTRATEGY_HPP */