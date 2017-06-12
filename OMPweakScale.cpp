// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// compile with: g++ OMPweakScale2.cpp -o ompWeak2 -fopenmp -std=c++11
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
  unsigned long long numTrials = 100000000;  // number of trials
  int numThreads;  // number of threads for parallel section
  int threadNum;  // number for each thread
  unsigned long long numHits;  // number of hits inside quarter circle
  double piEstimate;  // estimate of pi
  unsigned int xSeed, ySeed;  // seeds for random number generator
  unsigned long long i, k; int j, l;  // for loop counters
  for (numThreads = 1; numThreads < 17; ++numThreads) {
    std::vector<double> randomXvec(numTrials * numThreads);  // vector for x coordinates
    std::vector<double> randomYvec(numTrials * numThreads);  // vector for y coordinates
    std::vector<double> estimateVec(numThreads);  // vector for estimates of pi
    for (l = 0; l < 10; ++l) {
      xSeed = time(0); ySeed = numThreads;  // initialize seeds
      for (i = 0; i < numTrials * numThreads; ++i) {
        randomXvec[i] = rand_r(&xSeed) / (double)RAND_MAX;  // generate x coordinate
        randomYvec[i] = rand_r(&ySeed) / (double)RAND_MAX;  // generate y coordinate
      }
      auto start = std::chrono::steady_clock::now();  // gets time at beginning of program
      //#pragma omp parallel proc_bind(close) num_threads(numThreads)
      #pragma omp parallel for num_threads(numThreads) private(j,k,threadNum,numHits,piEstimate)
      for (j = 0; j < numThreads; ++j) {  // run one simulation on each thread
        threadNum = omp_get_thread_num();  // gets number of thread
        numHits = 0;  // resets number of hits
        for (k = threadNum * numTrials; k < (threadNum + 1) * numTrials; ++k) {
          if (randomXvec[k] * randomXvec[k] + randomYvec[k] * randomYvec[k] <= 1) {  // check if inside quarter circle
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
