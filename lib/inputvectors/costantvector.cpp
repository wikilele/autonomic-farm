#include <lib/inputvectors/inputvectors.hpp>


vector<int*>* getInputVector(){
    const int SECTION_SIZE = 3000;
    vector<int*>* returnvect = new vector<int*>();
    
    for(int i = 0; i < SECTION_SIZE; i ++ ){
        returnvect->push_back(new int(4));
    } 
}