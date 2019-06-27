#ifndef MONITOR_HPP
#define MONITOR_HPP
#include <src/monitor/MonitorStrategy.hpp>

class Monitor{

    IMonitorStrategy* mstrategy;

    public:

    Monitor(IMonitorStrategy* ms){
        mstrategy = ms;
    }

    void init(){

    }

    void notify(){

    }
};


#endif /* MONITOR_HPP */