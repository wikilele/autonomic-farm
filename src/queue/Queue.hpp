#include <mutex>
#include <string>
#include <condition_variable>
#include <deque>
#include <vector>
using namespace std;

template <typename T>
class Queue{
    private:
        std::mutex                  d_mutex;
        std::condition_variable     d_condition;
        std::deque<T>             d_queue;
    
    public:
        Queue(){}
        
        void push(T value) {
            {
                unique_lock<mutex> lock(this->d_mutex);
                d_queue.push_front(value);
            }
            this->d_condition.notify_one();
        }
        
        T pop(){
            unique_lock<mutex> lock(this->d_mutex);
            this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
            T retval(move(this->d_queue.back()));
            this->d_queue.pop_back();
            return retval;
        }

        vector<T> popAll(){
            vector<T> returnvect;
            {
                unique_lock<mutex> lock(this->d_mutex);
                for (auto i = d_queue.begin(); i != d_queue.end(); ++i) {
                    returnvect.push_back(d_queue.pop_front()); // TODO check if correct
                }
            }
            return returnvect;
        }
        
        bool is_empty(){
            unique_lock<mutex> lock(this->d_mutex);
            return(d_queue.empty());
        }
};

