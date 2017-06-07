// estimate pi using monte carlo methods
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

using namespace std;

int main()
{
  int numTrials;  // number of trials
  srand(time(0));  // initialize seed for random number generator
  int numRuns = 16;  // number of runs at each number of trials
  int insideCircle;  // number of hits inside quarter circle
  double x; double y;  // x and y coordinates of random points
  double distance;  // distance of point from origin
  double quarterCircleArea;  // estimate for area of quarter circle
  double piEstimate;  // estimate of pi
  double mean;  // mean of estimates for different numbers of trials
  double variance;  // variance of estimate
  double standardDev;  // standard deviation of estimates
  int i,j,a,b;  // for loop indices
  vector<double> estimateVec;  // vector of estimates of pi

  for (numTrials = 10; numTrials < 100000001; numTrials *= 10) {  // increment number of trials
	  
    estimateVec.clear();  // clear vector of estimates

    for (i = 0; i < 16; ++i) {  // repeat numRuns times

      // estimate pi using monte carlo methods
      insideCircle = 0;  // reset number of hits inside quarter circle
      for (j = 0; j < numTrials; ++j) {
        x = rand() / (double)RAND_MAX;  // generate x coordinate
        y = rand() / (double)RAND_MAX;  // generate y coordinate
        distance = sqrt(x * x + y * y);
        if (distance <= 1) {
          ++insideCircle;  // increment number of hits if inside circle
        }
      }
      // estimate pi and create vector of estimates
      quarterCircleArea = insideCircle / (double)numTrials;
      piEstimate = quarterCircleArea * 4;
      estimateVec.push_back(piEstimate);
    }

    mean = 0;  // mean of estimates
    for (a = 0; a < numRuns; ++a) {
      mean += estimateVec[a] / numRuns;
    }
    variance = 0;  // variance of estimates
    for (b = 0; b < numRuns; ++b) {
      variance += (estimateVec[b] - mean)*(estimateVec[b] - mean) / (numRuns - 1);
    }
    standardDev =  sqrt(variance);  // standard deviation of estimates
    cout << "For " << numTrials << " trials the mean estimate is " << mean
         << " and the standard deviation is " << standardDev << endl;
  }

  return 0;
}