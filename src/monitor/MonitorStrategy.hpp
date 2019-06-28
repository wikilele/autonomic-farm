#ifndef MONITORSTRATEGY_HPP
#define MONITORSTRATEGY_HPP

#define DONOTHING 0
#define ADDWORKER 2
#define REMOVEWORKER 4

/**
 * This tells to the monitor to reset the (starting) time point.
 * If that value is not refreshed periodically the actual threshold value might
 * be influenced by the previous computations leading to not so good values
 */
#define REFRESH_THRESHOLD 8
#define ADD_AND_REFRESH  (ADDWORKER | REFRESH_THRESHOLD)
#define REM_AND_REFRESH (REMOVEWORKER | REFRESH_THRESHOLD)


// TODO consider to define a threshold
// must be a float
#define TREND_THRESHOLD 0.10
#define AVG_THRESHOLD 0.10
// TODO maybe it's better if it's a faraction of the input size
// TODO the framework should care of set this automatically depending on some other parameters
#define WINDOW_SIZE 50

class IMonitorStrategy{
    protected:
    float expected_throughput;

    public:
        IMonitorStrategy(float et){
            expected_throughput = et;
        };
        virtual int addWorker(float actual_throughput) = 0;
};


class DefaultMonitorStrategy: public IMonitorStrategy{
    protected:       
        float past_throughputs[WINDOW_SIZE];
        int hystory_index = 0;
        
        // how many times the window has been filled 
        int phase = 0;

        /**
         * @return - it returns true if the window has been filled up
         */
        bool updateHystory( float actual_throughput){
            // adding event to hystory
            past_throughputs[hystory_index] = actual_throughput;

            // managing window and phases
            hystory_index ++;
            if(hystory_index >= WINDOW_SIZE) {
                phase ++;
                hystory_index = 0;
                return true;
            } else return false;
        }

        pair<float,float> computeTrendANDaverage(){
            float trend = 0;
            float average = past_throughputs[0];
            for (int i = 1; i < WINDOW_SIZE; i++){
                // the trend will be positive if the throughputs are increasing,
                // it will be negative otherwise
                trend = trend + (past_throughputs[i] - past_throughputs[i-1]);
                average = average + past_throughputs[i];
            }
            average = average / WINDOW_SIZE;

            return pair<float,float>(trend,average);
        }

        bool costantAverage(float average){
            return average >= expected_throughput - AVG_THRESHOLD && average <= expected_throughput + AVG_THRESHOLD;
        }

        bool lowAverage(float average){
            return average < expected_throughput - TREND_THRESHOLD;
        }

        bool highAverage(float average){
            return average > expected_throughput + TREND_THRESHOLD;
        }

        bool costantTrend(float trend){
            return trend >= - TREND_THRESHOLD && trend <= TREND_THRESHOLD;
        }

        bool increasingTrend(float trend){
            return trend > TREND_THRESHOLD;
        }

        bool decreasingTrend(float trend){
            return trend < - TREND_THRESHOLD;
        }

    public:
        DefaultMonitorStrategy(float exp_t): IMonitorStrategy(exp_t){};

        virtual int addWorker( float actual_throughput){
            bool switching_phases = updateHystory(actual_throughput);
            
            int command = DONOTHING;

            // taking decisions after having collected some throughputs
            if (switching_phases){
                pair<float,float> trendNaverage = computeTrendANDaverage();
                // unpacking the pair
                float trend = trendNaverage.first;
                float average = trendNaverage.second;

                if (costantAverage(average)){
                    // the average throughput is between a given threshold
                    // so there is nothing to do
                    command = DONOTHING;
                } else {
                    printf("trend %.2f - average %.2f \n",trend,average);

                    if (costantTrend(trend)){

                        if (lowAverage(average)) command = ADDWORKER;
                        else if (highAverage(average)) command = REMOVEWORKER;

                    } else if (increasingTrend(trend)){

                        if (lowAverage(average)) command = DONOTHING; // cause the trend is increasing
                        else if (highAverage(average)) command = REMOVEWORKER;

                    } else if (decreasingTrend(trend)){

                        if (lowAverage(average)) command = ADDWORKER;
                        else if (highAverage(average)) command= DONOTHING; // cause the trend is decreasing

                    }

                    command = command | REFRESH_THRESHOLD;
                }
            }

            return command;
        }
};




#endif /* MONITORSTRATEGY_HPP */