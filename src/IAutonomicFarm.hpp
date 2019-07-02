#ifndef IAUTONOMICFARM_HPP
#define IAUTONOMICFARM_HPP


template <typename TIN,typename TOUT>
class IAutonomicFarm{
    public:
        virtual vector<TOUT*>* runAndGetResults() = 0;
};

#endif /* IAUTONOMICFARM_HPP */