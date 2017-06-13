// collects data to demonstrate strong scaling (fixed problem size)
// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPstrongScale.cpp -o ompStrong -fopenmp -std=c++11
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <time.h>
#include <omp.h>

int main()
{
  unsigned long long numTrials = 100000000;  // number of trials
  int numThreads;  // number of threads for parallel section
  unsigned int xSeed, ySeed;  // seeds for random number generator
  unsigned long long numHits;  // number of hits inside quarter circle
  unsigned long long i, j;  // for loop counters
  std::vector<double> randomXvec(numTrials);  // vector for random x coordinates
  std::vector<double> randomYvec(numTrials);  // vector for random y coordinates
  for (numThreads = 1; numThreads < 17; ++numThreads) {
    for (int k = 0; k < 10; ++k) {
      xSeed =  time(0); ySeed =i;  // initialize seeds 
      for (i = 0; i < numTrials; ++i) {
        randomXvec[i] = rand_r(&xSeed) / (double)RAND_MAX;  // generate x coordinates
        randomYvec[i] = rand_r(&ySeed) / (double)RAND_MAX;  // generates y coordinates
      }
      numHits = 0;  // reset number of hits
      auto start = std::chrono::steady_clock::now();  // get time at beginning of program
      #pragma omp parallel for num_threads(numThreads) reduction(+:numHits)
      for (j = 0; j < numTrials; ++j) {
        if (randomXvec[j] * randomXvec[j] + randomYvec[j] * randomYvec[j] <= 1) {
          numHits += 1;
        }
      }
      double piEstimate = numHits / (double)numTrials * 4;  // calculates estimate of pi
      auto end = std::chrono::steady_clock::now();  // get time at end of execution
      std::chrono::duration<double> execTime = end - start;  // calculate execution time
      std::ofstream resultFile("OMPstrongScaling.csv", std::ios::out | std::ios::app);
      resultFile << piEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
    }
  }
  return 0;
}
