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
#include "vectorStandardDev.h"
#include "monteCarloPi.h"
#include <time.h>

using namespace std;

int main()
{
  int nTrials; // number of trials
  srand(time(0)); // initialize seed for random number generator

  for (nTrials = 10; nTrials < 100000001; nTrials *= 10) {  // increment number of trials
	  
    vector<double> estimates; // vector of estimates of pi
	  
    for (int j = 0; j < 10; ++j) {  // repeat 10 times

      // estimate value of pi
      double piEstimate;
      piEstimate = monteCarloPi(nTrials);
    
      // create vector of estimates
      estimates.push_back(piEstimate);
    }

    // calculate and display standard deviation
    double standardDev;
    standardDev = vectorStandardDev(estimates);
    cout << "For " << nTrials << " trials the standard deviation is " << standardDev << endl;
  }

  return 0;
}
	
	
