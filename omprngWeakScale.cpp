// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPweakScale2.cpp -o ompWeak2 -I/home/jgoncalves/repos/omprng -fopenmp -std=c++11
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <omp.h>
#include "omprng.h"

int main()
{
  unsigned long long numTrials = 1000000;  // number of trials
  int numThreads;  // number of threads for parallel section
  int threadNum;  // number for each thread
  double x, y;  // random x and y coordinates
  unsigned long long numHits;  // number of hits inside quarter circle
  double piEstimate;  // estimate of pi
  unsigned long long k; int i, j;  // for loop counters
  omprng rng;  // initiliaze random number generator
  for (numThreads = 1; numThreads < 17; ++numThreads) {
    rng.setNumThreads(numThreads);  // set number of threads for parallel section
    std::vector<double> estimateVec(numThreads);  // vector for estimates of pi
    for (i = 0; i < 10; ++i) {
      auto start = std::chrono::steady_clock::now();  // gets time at beginning of program
      #pragma omp parallel for num_threads(numThreads) private(k,x,y,threadNum,numHits,piEstimate)
      for (j = 0; j < numThreads; ++j) {  // run one simulation on each thread
        threadNum = omp_get_thread_num();  // gets number of thread
        numHits = 0;  // resets number of hits
        for (k = numTrials; k < numTrials; ++k) {
          x = rng.runif();  // generate random x coordinate
          std::cout << x << std::endl;
          y = rng.runif();  // generate random y coordinate
          if (x * x + y * y <= 1) {  // check if inside quarter circle
            ++numHits;  // +1 hit inside circle
          }
        }
        piEstimate = numHits / (double)numTrials * 4;  // estimates value of pi
        estimateVec[threadNum] = piEstimate;  // save estimate to estimateVec
      }
      double mean = 0;  // mean of estimates
      for (int a = 0; a < numThreads; ++a) {
        mean += estimateVec[a] / numThreads;
      }
      auto end = std::chrono::steady_clock::now();  // gets time at end of program
      std::chrono::duration<double> execTime = end - start;  // calculates execution time
      std::ofstream resultFile("OMPweakScaling2.csv", std::ios::app);  // opens file to append results
      resultFile << mean << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;  // write results to file
    }
  }
  return 0;
}
