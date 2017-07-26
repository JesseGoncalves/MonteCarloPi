#include <random>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <omp.h>

double rand01()
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count() + omp_get_thread_num();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}


int main()
{
  static unsigned long long numTrials = 10e8;  // number of trials for monte carlo simulation

  int maxThreads = omp_get_max_threads();  // maximum number of threads to run concurrently

  int numThreads;  // number of threads for parallel section

  unsigned long long trialsPerThread;  // number of trials run on each thread

  double x, y;  // random x and y coordinates

  unsigned long long numHits;  // number of points generated inside circle

  double piEstimate;  // estimate of pi

  double meanEstimate;  // mean of all estimates

  int i, j, l; unsigned long long k;  // for loop counters

  for (numThreads = 1; numThreads < maxThreads + 1; ++numThreads) {  // iterate for different numbers of threads

    for (i = 0; i < 10; ++i) {  // repeat 10 times at each number of threads

      std::vector<double> estimateVec(numThreads);  // vector for estimates of pi

      trialsPerThread = (unsigned long long)(numTrials / (double)numThreads);  // set trials per thread

      meanEstimate = 0;  // reset mean estimate

      auto start = std::chrono::steady_clock::now();  // get time at beginning of execution

      #pragma omp parallel for num_threads(numThreads) private(k,x,y,numHits,piEstimate)

      for (j = 0; j < numThreads; ++j) {

        numHits = 0;  // reset number of hits inside circle

        for (k = 0; k < trialsPerThread; ++k) {
   
          x = rand01(); y = rand01();  // generate random x and y coordinates
 
          if (x * x + y * y < 1) {  

            numHits += 1;  // increment numHits if point is inside circle
  
          }

        }

        piEstimate = 4 * numHits / (double)trialsPerThread;  // calculate estimate of pi
                 
        estimateVec[omp_get_thread_num()] = piEstimate;  // store estimate in vector

      }
   
      for (l = 0; l < numThreads; ++l) {

        meanEstimate += estimateVec[l] / numThreads;

      }
    
      auto end = std::chrono::steady_clock::now();  // get time at end of execution

      std::chrono::duration<double> execTime = end - start;  // calculate execution time

      std::ofstream resultFile("OMPstrongScaling.csv", std::ios::out | std::ios::app);

      resultFile << meanEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;

    }

  }
  
  return 0;

}  
