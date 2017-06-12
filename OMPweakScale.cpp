// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPweakScale.cpp -o ompWeak -fopenmp -std=c++11
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <omp.h>

int main()
{
  for (int k = 1; k < 17; ++k) {
    auto start = std::chrono::steady_clock::now();  // gets time at beginning of program
    int numThreads = k;  // number of threads for parallel section
    int threadNum;  // number for each thread
    unsigned long long numTrials = 1000000000;  // number of trials
    double x;  // random x coordinates
    double y;  // random y coordinates
    unsigned long long numHits;  // number of hits inside quarter circle
    double piEstimate;  // estimate of pi
    int i; int j;  // for loop counters
    unsigned int seed;  // seed for random number generator
    std::vector<double> estimateVec(numThreads, 0);  // vector of zeros to hold estimates of pi
    //#pragma omp parallel proc_bind(close) num_threads(numThreads)
    #pragma omp parallel for num_threads(numThreads) private(i,j,threadNum,seed,x,y,numHits,piEstimate)
    for (i = 0; i < numThreads; ++i) {  // run one simulation on each thread
      threadNum = omp_get_thread_num();  // gets number of thread
      seed = threadNum;  // sets different seed for each thread
      numHits = 0;  // resets number of hits
      for (j = 0; j < numTrials; ++j) {
        x = rand_r(&seed) / (double)RAND_MAX;  // generate random x coordinate
        y = rand_r(&seed) / (double)RAND_MAX;  // generate random y coordinate
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
    std::ofstream resultFile("OMPweakScaling.csv", std::ios::app);  // opens file to append results
    resultFile << mean << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;  // write results to file
  }
  return 0;
}
