/**
 *  This file will provide the function and the input vector to
 *  test the AutonomicFarm
 * 
 *  This input are those ones used to run the ecperiment on the xeon phi
 *  machine
 */
#include <iostream>   
#include <vector>
#include <chrono>
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


vector<int*>* getInputVector();
