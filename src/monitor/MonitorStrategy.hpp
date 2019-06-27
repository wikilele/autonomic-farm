#ifndef MONITORSTRATEGY_HPP
#define MONITORSTRATEGY_HPP

#include <stdlib.h>     /* srand, rand */
#include <time.h>

#define ADDWORKER 1
#define REMOVEWORKER -1
#define DONOTHING 0

// TODO consider to define a threshold
// must be a float
#define TREND_THRESHOLD 0.10
#define AVG_THRESHOLD 0.10


class IMonitorStrategy{
    protected:
    float expected_throughput;

    public:
        IMonitorStrategy(float et){
            expected_throughput = et;
        };
        virtual int addWorker(float actual_throughput) = 0;
};

class DumbMonitorStrategy: public IMonitorStrategy{
    public:
        DumbMonitorStrategy(float exp_t): IMonitorStrategy(exp_t){};

        virtual int addWorker(float actual_throughput){
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

// TODO maybe it's better if it's a faraction of the input size
#define WINDOW_SIZE 50

class DefaultMonitorStrategy: public IMonitorStrategy{
    protected:       
        float past_throughputs[WINDOW_SIZE];
        int hystory_index = 0;
        
        // how many times the window has been filled 
        int phase = 0;

    public:
        DefaultMonitorStrategy(float exp_t): IMonitorStrategy(exp_t){};
        virtual int addWorker( float actual_throughput){
            bool switching_phases = false;
            // adding event to hystory
            past_throughputs[hystory_index] = actual_throughput;

            // managing window and phases
            hystory_index ++;
            if(hystory_index >= WINDOW_SIZE) {
                phase ++;
                hystory_index = 0;
                switching_phases = true;
            }

            // taking decisions after having collected some throughputs
            if (switching_phases){
                float trend = 0;
                float average = past_throughputs[0];
                for (int i = 1; i < WINDOW_SIZE; i++){
                    // the trend will be positive if the throughputs are increasing,
                    // it will be negative otherwise
                    trend = trend + (past_throughputs[i] - past_throughputs[i-1]);
                    average = average + past_throughputs[i];
                }
                average = average / WINDOW_SIZE;

                if (average >= expected_throughput - AVG_THRESHOLD && average <= expected_throughput + AVG_THRESHOLD){
                    // the average throughput is between a given threshold
                    // so there is nothing to do
                    return DONOTHING;
                }

                printf("trend %.2f - average %.2f \n",trend,average);

                if (trend >= - TREND_THRESHOLD && trend <= TREND_THRESHOLD){
                    //costant trend
                    if (average < expected_throughput - TREND_THRESHOLD){
                        return ADDWORKER;
                    } else if (average > expected_throughput + TREND_THRESHOLD){
                        return REMOVEWORKER;
                    }
                } else if (trend > TREND_THRESHOLD){
                    // increasing trend
                    if (average < expected_throughput - TREND_THRESHOLD){
                        return DONOTHING; // cause the trend is increasing
                    } else if (average > expected_throughput + TREND_THRESHOLD){
                        return REMOVEWORKER;
                    }
                } else if (trend < - TREND_THRESHOLD){
                    // decreasing trend
                    if (average < expected_throughput - TREND_THRESHOLD){
                        return ADDWORKER; 
                    } else if (average > expected_throughput + TREND_THRESHOLD){
                        return DONOTHING; // cause the trend is decreasing
                    }
                }

            } else return DONOTHING;
        }
};




#endif /* MONITORSTRATEGY_HPP */