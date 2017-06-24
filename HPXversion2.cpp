#include <cstddef>
#include <cstdint>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <thread>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/parallel_for_loop.hpp>

double rand01()
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}

double trials()
{
  static int numTrials = 1000;  // number of trials per thread
  
  int numHits = 0;  // number of hits inside circle 

  for (int i = 0; i < numTrials; ++i) {

    double x = rand01(); double y = rand01();  // generate random x and y coordinates

    if (x * x + y * y < 1) {

      numHits += 1;  // increment numHits if point is inside circle
   
    }
 
  }
  
  return 4 * numHits / (double) numTrials;  // return estimate of pi
}

void calculate_pi()
{
  static int numTrials = 1000;  // number of trials per thread

  static int numThreads = 1000;  // number of threads to be executed

  double estimate = 0;  // estimate of pi

  auto start = std::chrono::steady_clock::now();  // time at beginning of execution
 
  for_loop(par, 0, numThreads, reduction_plus(estimate), [&](int j, double estimate) {

    estimate = hpx::async(trials);  // return estimate of pi from each thread 

  }

  double piEstimate = estimate / numThreads;  // calculate overall estimate of pi

  auto end = std::chrono::steady_clock::now();  // time at end of execution

  std::chrono::duration<double> execTime = end - start;  // execution time

  std::ofstream resultFile("HPXdata.csv", std::ios::out | std::ios::app);  // write results to .csv file

  resultFile << piEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
}

