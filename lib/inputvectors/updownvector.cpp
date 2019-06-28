#include <lib/inputvectors/inputvectors.hpp>


vector<int*>* getInputVector(){
    const int SECTION_SIZE = 50;
    vector<int*>* returnvect = new vector<int*>();
    
    for(int i = 0; i < SECTION_SIZE; i ++){
        for(int i = 0; i < SECTION_SIZE; i ++ ){
            returnvect->push_back(new int(4));
        } 
        
        for(int i = 0; i < SECTION_SIZE; i ++ ){
            returnvect->push_back(new int(1));
        }

        for(int i = 0; i < SECTION_SIZE; i ++ ){
            returnvect->push_back(new int(8));
        }
    }
    return returnvect;
}