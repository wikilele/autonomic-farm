#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP


#include <vector>
using namespace std;

// Interface for implementing Collector
template <typename TOUT>
class ICollector{

    public:
        // ASSUME no concurrent call 
        virtual void pushResult(TOUT* res) = 0;

        virtual vector<TOUT*>*  returnResults() = 0;
};


template <typename TOUT>
class DefaultCollector: public ICollector<TOUT>{
    protected:
        vector<TOUT*> results_vector;
    
    public:
        void pushResult(TOUT* res){
           results_vector.push_back(res);
        }

        vector<TOUT*>*  returnResults(){
           return new vector<TOUT*>(results_vector); 
        }
};

#endif /* COLLECTOR_HPP */