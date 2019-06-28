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
#define REFRESH_THROUGHPUT 8



class IMonitorStrategy{
    protected:
    float expected_throughput;

    public:
        IMonitorStrategy(float et){
            expected_throughput = et;
        };
        virtual int addWorker(float actual_throughput) = 0;
};


#endif /* MONITORSTRATEGY_HPP */