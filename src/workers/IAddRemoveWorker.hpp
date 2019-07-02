#ifndef IADDREMOVEWORKER_HPP
#define IADDREMOVEWORKER_HPP

class IAddRemoveWorker{
    public:
        virtual void addWorker() = 0;
        virtual void removeWorker() = 0;

        virtual int getActualWorkers()= 0;
        virtual int getTotalWorkers() = 0;
};

#endif /* IADDREMOVEWORKER_HPP */