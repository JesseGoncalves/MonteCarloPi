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
  auto start = std::chrono::steady_clock::now();  // gets time at beginning of program
  int numThreads = 16;  // number of threads for parallel section
  int threadNum;  // number for each thread
  unsigned long long numTrials = 100000000;  // number of trials
  double randomX;  // random x coordinates
  double randomY;  // random y coordinates
  unsigned long long insideCircle;  // number of hits inside quarter circle
  double quarterCircleArea;  // approximated with probability of being inside circle
  double piEstimate;  // estimate of pi
  int i; int j;  // for loop counters
  unsigned int seed;  // seed for random number generator
  std::vector<double> estimateVec(numThreads, 0);  // vector of zeros to hold estimates of pi
  #pragma omp parallel for num_threads(numThreads) private(i,j,threadNum,seed,randomX,randomY,insideCircle,quarterCircleArea,piEstimate)
    for (i = 0; i < numThreads; ++i) {  // run one simulation on each thread
      threadNum = omp_get_thread_num();  // gets number of thread
      seed = threadNum;  // sets different seed for each thread
      insideCircle = 0;  // resets number of hits
      for (j = 0; j < numTrials; ++j) {
        randomX = rand_r(&seed) / (double)RAND_MAX;  // generate random x coordinate
        randomY = rand_r(&seed) / (double)RAND_MAX;  // generate random y coordinate
        if (randomX * randomX + randomY * randomY <= 1) {  // check if inside quarter circle
          ++insideCircle;  // +1 hit inside circle
        }
      }
      quarterCircleArea = insideCircle / (double)numTrials;  // estimates area with probability
      piEstimate = quarterCircleArea * 4;  // estimates value of pi
      estimateVec[threadNum] = piEstimate;  // save estimate to estimateVec
    }
  double mean = 0;  // mean of estimates
  for (int a = 0; a < numThreads; ++a) {
      mean += estimateVec[a] / numThreads;
  }
  auto end = std::chrono::steady_clock::now();  // gets time at end of program
  std::chrono::duration<double> execTime = end - start;  // calculates execution time
  std::ofstream resultFile("OMPweakScaleResults.csv", std::ios::app);  // opens file to append results
  resultFile << numTrials << ", " << numThreads << ", " << mean << ", " << execTime << std::endl;  // write results to file
  return 0;
}
