/**
 *  This file will provide the function and the input vector to
 *  test the AutonomicFarm
 * 
 *  This input are those ones used to run the ecperiment on the xeon phi
 *  machine
 */
#include <iostream>   
#include <chrono>  
#include <vector>
using namespace std;

auto activewait = [](int *x) ->int*{
    auto start = std::chrono::high_resolution_clock::now();
    while (true)  {
      auto elapsed =  std::chrono::high_resolution_clock::now() - start;
      long long microseconds =  std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
      if (microseconds >= 1000* (*x)) break;
    }
    return x;
};


vector<int*>* getDefaultInputVector(){
    const int SECTION_SIZE = 1000;
    vector<int*>* returnvect = new vector<int*>();
    
    for(int i = 0; i < SECTION_SIZE; i ++ ){
        returnvect->push_back(new int(4));
    } 
    
    for(int i = 0; i < SECTION_SIZE; i ++ ){
        returnvect->push_back(new int(1));
    }

    for(int i = 0; i < SECTION_SIZE; i ++ ){
        returnvect->push_back(new int(8));
    }
    
    return returnvect;
}


vector<int*>* getUpDownInputVector(){
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