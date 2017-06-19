// estimates value of pi using monte carlo methods
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <vector>
#include <time.h>

int main()
{
  unsigned long long numTrials = 100000000;  // number of trials
  unsigned long long numHits;  // number of hits inside circle
  unsigned int xSeed, ySeed;  // seeds for random number generator
  std::vector<double> randomXvec(numTrials);  // vector of random x coordinates
  std::vector<double> randomYvec(numTrials);  // vector of y coordinates
  double piEstimate;  // estimate of pi
  int i; unsigned long long j, k;  // for loop counters
  for (i = 0; i < 10; ++i) {
    numHits = 0;  // reset each iteration
    xSeed = time(0); ySeed = i;  // initialize seeds
    for (j = 0; j < numTrials; ++j) {
      randomXvec[j] = rand_r(&xSeed) / (double)RAND_MAX;  // generate random x coordinates
      randomYvec[j] = rand_r(&ySeed) / (double)RAND_MAX;  // generate random y coordinates
    }
    auto start = std::chrono::steady_clock::now();  // get time at beginning of execution   
    for (k = 0; k < numTrials; ++k) {
      if (randomXvec[k] * randomXvec[k] + randomYvec[k] * randomYvec[k] <= 1) {
        ++numHits;
      }
    }
    piEstimate = numHits / (double)numTrials * 4;  // calculates estimate of pi
    auto end = std::chrono::steady_clock::now();  // get time at end of execution
    std::chrono::duration<double> execTime = end - start;  // calculates execution time
    std::ofstream resultFile("serialData2.csv", std::ios::app);  // opens file to append results
    resultFile << piEstimate << ", " << numTrials << ", " << execTime.count() << std::endl; // write results to file
  }
  return 0;
}
