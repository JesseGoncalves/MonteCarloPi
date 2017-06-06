// estimate pi using monte carlo methods, parallelized with OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// test precision by running code 10 times for 10^n trials,
// where n={1,2,3,4,5,6}, and calculating standard deviations
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <omp.h>
#include <stdlib.h>

using namespace std;

int main()
{
  int numThreads = 16;  // number of threads for parallel section
  int threadNum;  // number for each thread
  int numTrials;  // number of trials
  double randomX;  // random x coordinates
  double randomY;  // random y coordinates
  int insideCircle;  // number of hits inside quarter circle
  double quarterCircleArea;  // approximated with probability of being inside circle
  double piEstimate;  // estimate of pi
  int i; int j;  // for loop counters
  unsigned int seed;  // seed for random number generator
  for (numTrials = 10; numTrials < 100000001; numTrials *= 10) {  // increment number of trials
    vector<double> estimateVec(numThreads, 0);  // vector of zeros to hold estimates of pi
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
      quarterCircleArea = insideCircle / (double)numTrials;  // estimates area with probability of hitting it
      piEstimate = quarterCircleArea * 4;  // estimates value of pi
      estimateVec[threadNum] = piEstimate;  // save estimate to estimateVec
    }
    double mean = 0;  // mean of estimates
    for (int a = 0; a < numThreads; ++a) {
        mean += estimateVec[a] / numThreads;
    }
    double variance = 0;  // variance of estimates
    for (int b = 0; b < numThreads; ++b) {
      variance += (estimateVec[b] - mean) * (estimateVec[b] - mean) / (numThreads - 1);
    }
    double standardDev = sqrt(variance);  // standard deviation of estimates
    cout << "For " << numTrials << " trials the mean estimate is " << mean
         << " and the standard deviation is " << standardDev << endl;  // display results
  }
  return 0;
}
