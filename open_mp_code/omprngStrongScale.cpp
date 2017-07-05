// collects data to demonstrate strong scaling (fixed problem size)
// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPstrongScale.cpp -o ompStrong -fopenmp -std=c++11
#include <fstream>
#include <vector>
#include <chrono>
#include <time.h>
#include <omp.h>
#include "omprng.h"

int main()
{
  unsigned long long numTrials = 100000000;  // number of trials
  int numThreads;  // number of threads for parallel section
  int trialsPerThread;  // number of trials per thread
  unsigned long long numHits;  // number of hits inside quarter circle
  unsigned long long k; int i, j, l;  // for loop counters
  double x, y;  // random x and y coordinates
  double piEstimate;  // estimate of pi
  double meanEstimate;  // final estimate of pi
  //std::vector<double> randomXvec(numTrials);  // vector for random x coordinates
  //std::vector<double> randomYvec(numTrials);  // vector for random y coordinates
  for (numThreads = 1; numThreads < 17; ++numThreads) {
    for (i = 0; i < 10; ++i) {
      std::vector<double> estimateVec(numThreads);  // vector of estimates
      numHits = 0;  // reset number of hits
      trialsPerThread = (int)(numTrials / (double)numThreads);  // set number of trials per thread
      std::cout << trialsPerThread << std::endl;
      omprng rng;  // initialize random number generator
      rng.setNumThreads(numThreads);
      auto start = std::chrono::steady_clock::now();  // get time at beginning of program
      #pragma omp parallel for num_threads(numThreads) private(k,x,y,numHits,piEstimate)
      for (j = 0; j < numThreads; ++j) {
        for (k = 0; k < trialsPerThread; ++k) { 
          x = rng.runif();  // generate x coordinate
          y = rng.runif();  // generate y coordinate
          if (x * x + y * y < 1) {
            numHits += 1;
          }
         // randomXvec[k] = rng.runif();  // generate x coordinates
         // randomYvec[k] = rng.runif();  // generate y coordinates
        }
        piEstimate = 4 * numHits / (double)trialsPerThread;  // calculates estimate of pi
        estimateVec[omp_get_thread_num()] = piEstimate;  // stores estimate in vector
      }
      //#pragma omp for reduction(+:numHits)
      //for (k = 0; k < numTrials; ++k) {
      //  if (randomXvec[k] * randomXvec[k] + randomYvec[k] * randomYvec[k] <= 1) {
      //    numHits += 1;
      //  }
      //}
      meanEstimate = 0;
      for (l = 0; l < numThreads; ++l) {
        meanEstimate += estimateVec[l] / numThreads;
      }
      auto end = std::chrono::steady_clock::now();  // get time at end of execution
      std::chrono::duration<double> execTime = end - start;  // calculate execution time
      std::ofstream resultFile("omprngStrongScaling.csv", std::ios::out | std::ios::app);
      resultFile << meanEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
    }
  }
  return 0;
}
