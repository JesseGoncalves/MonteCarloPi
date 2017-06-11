// collects data to demonstrate strong scaling (fixed problem size)
// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPstrongScale.cpp -o ompStrong -fopenmp -openmp-libcompat -std=c++11
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include <time.h>
#include <omp.h>

int main()
{
  for (int j = 1; j < 17; ++j) {
  auto start = std::chrono::steady_clock::now();  // get time at beginning of program
  int numThreads = j;  // number of threads for parallel section
  unsigned long long numTrials = 100000000;  // number of trials
  unsigned int seed = time(0);  // initialize seed for random number generator
  double x;  // random x coordinates
  double y;  // random y coordinates
  unsigned long long numHits = 0;  // number of hits inside quarter circle
  unsigned long long i;  // for loop counter
  //GOMP_AFFINITY = 0:numThreads;  // assigns threads to specific processors
  #pragma omp parallel proc_bind(close) num_threads(numThreads)
  #pragma omp for private(x,y) reduction(+:numHits)
  for (i = 0; i < numTrials; ++i) {
    x = rand_r(&seed) / (double)RAND_MAX;  // generate random x coordinate
    y = rand_r(&seed) / (double)RAND_MAX;  // generate random y coordinate
    if (x * x + y * y <= 1) {
      numHits += 1;
    }
  }
  double piEstimate = numHits / (double)numTrials * 4;  // calculates estimate of pi
  auto end = std::chrono::steady_clock::now();  // get time at end of execution
  std::chrono::duration<double> execTime = end - start;  // calculate execution time
  std::ofstream resultFile("OMPstrongScaling.csv", std::ios::out | std::ios::app);
  resultFile << piEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
  }
  return 0;
}
