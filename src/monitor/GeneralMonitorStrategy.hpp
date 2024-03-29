#ifndef DEFAULTMONITORSTRATEGY_HPP
#define DEFAULTMONITORSTRATEGY_HPP


#include <src/monitor/MonitorStrategy.hpp>
#include<vector>
#include<iostream>
using namespace std;


class GeneralMonitorStrategy: public IMonitorStrategy{
    protected:
        // this values must be untouched
        int WINDOW_SIZE;
        float AVG_THRESHOLD;
        float TREND_THRESHOLD;

        vector<float>* past_throughputs;
        int hystory_index = 0;
        
        // how many times the window has been filled 
        int phase = 0;

        int old_command = DONOTHING;

        /**
         * @return - it returns true if the window has been filled up
         */
        bool updateHystory( float actual_throughput){
            // adding event to hystory
            (*past_throughputs)[hystory_index] = actual_throughput;

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
            float average = (*past_throughputs)[0];
            for (int i = 1; i < WINDOW_SIZE; i++){
                // the trend will be positive if the throughputs are increasing,
                // it will be negative otherwise
                trend = trend + ((*past_throughputs)[i] - (*past_throughputs)[i-1]);
                average = average + (*past_throughputs)[i];
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

        int avoidConsecutiveDisaccordingCommands(int command){
             // this should avoid to do an ADD and a consecutive REMOVE
            // ex.  old_command == 0100 == REMOVEWORKER
            //      command == 1010 == ADDWORKER | REFRESH_THROUGHPUT
            //      command = 1000 = 1010 & 1101 = 1010 & ~ADDWORKER 
            if(old_command & REMOVEWORKER)
                command = command & ~ADDWORKER;
            else if (old_command & ADDWORKER)
                command = command & ~REMOVEWORKER;
            
            //      old_command = 0000 = 1000 & 0111 = 1000 & ~REFRESH_THROUGHPUT
            //      this may reset the old_command to DONOTHING so that next time everything is allowed
            old_command = command & ~REFRESH_THROUGHPUT;

            return command;
        }

    public:
        GeneralMonitorStrategy(float exp_t, int inputsize): IMonitorStrategy(exp_t){
            // defining a window back in time
            this->WINDOW_SIZE = inputsize/150; 
            // defining some thresholds
            this->AVG_THRESHOLD = exp_t* 1/10;
            this->TREND_THRESHOLD = exp_t* 2/10;
            past_throughputs = new vector<float>(WINDOW_SIZE);
        }

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
#ifdef DEBUG
                    printf("trend %.2f - average %.2f \n",trend,average);
#endif /* DEBUG */
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

                    command = command | REFRESH_THROUGHPUT;
                }

               command = avoidConsecutiveDisaccordingCommands(command);
            }

            return command;
        }
};


#endif /* DEFAULTMONITORSTRATEGY_HPP */