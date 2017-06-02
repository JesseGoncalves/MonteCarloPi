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
#include <time.h>
#include <omp.h>

using namespace std;

int main()
{
  int numThreads = 16;  // number of threads for parallel section
  int numTrials;  // number of trials
  srand(time(0));  // initialize seed for random number generator
  double randomX;  // random x coordinates
  double randomY;  // random y coordinates
  for (numTrials = 10; numTrials < 100000001; numTrials *= 10) {  // increment number of trials
    vector<double> randomVecX;  // empty vector to hold randomly generated x coordinates
    vector<double> randomVecY;  // empty vector to hold randomly generated y coordinates
    for (int i  = 0; i < numTrials * numThreads; ++i) {
      randomX = rand() / (double)RAND_MAX;  // generate random x coordinate
      randomVecX.push_back(randomX);  // add random x coordinate to vector
      randomY = rand() / (double)RAND_MAX;  // generate random y coordinate
      randomVecY.push_back(randomY);  // add random y coordinate to vector
    }
    int results;  // number of hits inside quarter circle
    double distance;  // distance from origin to point
    double quarterCircleArea;  // approximated with probability of being inside circle
    double piEstimate;  // estimate of pi
    vector<double> estimateVec(numThreads, 0);  // vector of zeros to hold estimates of pi 
    #pragma omp parallel for private(results,distance,quarterCircleArea,piEstimate) num_threads(numThreads)
    for (int j = 0; j < numThreads; ++j) {  // run one simulation on each thread
      results = 0;
      for (int k = omp_get_thread_num()*numTrials; k < omp_get_thread_num()*numTrials+numTrials; ++k) {
        distance = sqrt(pow(randomVecX[k], 2) + pow(randomVecY[k], 2));
        if (distance <= 1) {  // check if inside quarter circle
          ++results;
        }
      }
      quarterCircleArea = (double)results / numTrials;
      piEstimate = quarterCircleArea * 4;
      estimateVec[omp_get_thread_num()] = piEstimate;  // save estimate to estimateVec
    }
    double mean = 0;  // mean of estimates
    for (int a = 0; a < numThreads; ++a) {
      mean += estimateVec[a] / numThreads;
    }
    double variance = 0;  // variance of estimates
    for (int b = 0; b < numThreads; ++b) {
      variance += pow(estimateVec[b] - mean, 2) / (numThreads - 1);

    }
    double standardDev = sqrt(variance);  // standard deviation of estimates
    cout << "For " << numTrials << " trials the mean estimate is " << mean
         << " and the standard deviation is " << standardDev << endl;  // display results
  }
  return 0;
}
