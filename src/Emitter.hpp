#include <vector>
using namespace std;

// Iterface for implementing Emitter
template <typename TIN>
class IEmitter{

    public:
        // ASSUME no concurrent call
        virtual TIN* getNextItem() = 0;

};


template <typename TIN>
class DefaultEmitter: public IEmitter<TIN> {
    protected:
        // taking a pointer to avoid copying huge vectors
        vector<TIN>* stream_vector;
        int vector_index;

    public:
        DefaultEmitter(vector<TIN>* vect){
            stream_vector = vect;
            vector_index = 0;
        }

        TIN* getNextItem(){
            TIN* item  = NULL; // TODO consider using defined EOS
            
            if(vector_index < stream_vector->size()){
                
                item = &(stream_vector->at(vector_index));
                vector_index ++;
            }
            
            return item;
        }

};