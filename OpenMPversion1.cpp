// estimate pi using monte carlo methods
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// test precision by running code 10 times for 10^n trials,
// where n={1,2,3,4,5,6}, and calculating standard deviations
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<vector>

using namespace std;

int main()
{
  int nTrials; // number of trials

  for (nTrials = 10; nTrials < 100000001; nTrials *= 10) {  // increment number of trials
	  
    vector<double> estimates; // vector of estimates of pi
	  
    for (int j = 0; j < 10; ++j) {  // repeat 10 times

      // generate and classify random points
      int results = 0; // number of hits in circle
      for (int i = 0; i < nTrials; ++i) {
        double x = rand() / (double)RAND_MAX; // x coord
        double y = rand() / (double)RAND_MAX; // y coord
        double distance = sqrt(pow(x, 2) + pow(y, 2)); // distance from origin
        if (distance <= 1) {
          ++results;
        }
      }
 
      // estimate area of quarter circle
      double quarterCircleArea = (double)results / (double)nTrials; 

      // calculate estimation of pi
      double piEstimate = quarterCircleArea * 4;
    
      // create vector of estimates
      estimates.push_back(piEstimate);
    }
    
    // calculate mean and variance
    double mean = 0;  // mean of estimates
    for (int a = 0; a < 10; ++a) {
      mean += estimates[a] / 10;
    }
    double variance = 0;  // variance of estimates
    for (int b = 0; b < 10; ++b) {
      variance += pow(estimates[b] - mean, 2) / 10;
    }
	  
    // calculate and display standard deviation
    double standardDev = pow(variance, .5);
    cout << "For " << nTrials << " trials the standard deviation is " << standardDev << endl;
  }

  return 0;
}
