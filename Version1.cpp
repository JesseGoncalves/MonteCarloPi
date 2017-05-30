// estimate pi using monte carlo methods
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
// test precision by running code 100 times for 10^n trials,
// for n={1,2,3,4,5,6}, and calculating standard deviations
#include<cstdlib>
#include<cmath>
#include<iostream>

using namespace std;

int main()
{
  int nTrials; // number of trials
  for (nTrials = 10; nTrials < 1000001; nTrials *= 10) {  // increment number of trials
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
    
      // calculate variance
      double variance = 0;
      variance += piEstimate**2 / 10;
    }
    
    // calculate and display standard deviation
    double standardDev = variance**.5;
    cout << "For " << nTrials << " trials the standard deviation is " << standardDev << endl;
  }

  return 0;
}
	
	
