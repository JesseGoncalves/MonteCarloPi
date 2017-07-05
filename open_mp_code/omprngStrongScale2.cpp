// collects data to demonstrate strong scaling (fixed problem size)
// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPstrongScale2.cpp rngstream.cpp -o ompStrong -I/home/jgoncalves/repos/omprng -fopenmp -std=c++11
#include <fstream>
#include <chrono>
#include <vector>
#include <omp.h>
#include "omprng.h"

int main()
{
  unsigned long long numTrials = 10000000;  // number of trials
  int numThreads;  // number of threads for parallel section
  double x, y;  // random x and y coordinates
  unsigned long long numHits;  // number of hits inside quarter circle
  unsigned long long i, j;  // for loop counters
  for (numThreads = 1; numThreads < 17; ++numThreads) {
    for (int k = 0; k < 10; ++k) {
      omprng rng;  // initialize random number generator
      rng.setNumThreads(numThreads);
      numHits = 0;  // reset number of hits
      auto start = std::chrono::steady_clock::now();  // get time at beginning of program
      #pragma omp parallel num_threads(numThreads) private(x,y) reduction(+:numHits)
      for (j = 0; j < numTrials; ++j) {
        x = rng.runif();  // generate random x coordinate
        y = rng.runif();  // generate random y coordinate
        if (x * x + y * y <= 1) {
          numHits += 1;
        }
      }
      double piEstimate = numHits / (double)numTrials * 4;  // calculates estimate of pi
      auto end = std::chrono::steady_clock::now();  // get time at end of execution
      std::chrono::duration<double> execTime = end - start;  // calculate execution time
      std::ofstream resultFile("omprngStrongScaling2.csv", std::ios::out | std::ios::app);
      resultFile << piEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
    }
  }
  return 0;
}
