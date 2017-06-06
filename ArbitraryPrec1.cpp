// estimate value of pi to arbitrary precision using monte carlo methods
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <gmp.h>

using namespace std;

int main() 
{
  srand(time(0));  // initialize seed for random number generator
  mpf_set_default_prec(64);
  int numTrials = 1000000000;  // number of trials for monte carlo method
  double x,y;  // random x and y coordinates
  double distance;  // distance from origin to random point
  int numHits = 0;  // number of points inside of quarter circle (distance <= 1)
  mpf_t quarterCircleArea;  // estimate for area of quarter circle
  mpf_init(quarterCircleArea);
  mpf_t piEstimate;  // estimate of pi
  mpf_init(piEstimate);
  for (int i = 0; i < numTrials; ++i) {
    x = rand() / (double)RAND_MAX;  // generate x coordinate
    y = rand() / (double)RAND_MAX;  // generate y coordinate
    distance = sqrt(x * x + y * y);
    if (distance <= 1) {
      ++numHits;  // increment number of hits if inside circle
    }
  }
  mpf_t numTrialsMP;  // multiple precision float type for numTrials
  mpf_init(numTrialsMP);
  mpf_set_ui(numTrialsMP, numTrials);  // gives numTrials multiple precision
  mpf_t numHitsMP;  // multiple precision float type for numHits
  mpf_init(numHitsMP);
  mpf_set_ui(numHitsMP, numHits);  // give numHits multiple precision
  mpf_div(quarterCircleArea, numHitsMP, numTrialsMP);  // estimates area of quarter circle
  mpf_mul_ui(piEstimate, quarterCircleArea, 4);  // calculates estimate of pi
  mpf_out_str(stdout, 10, 0, piEstimate);  // display estimate of pi
  cout << endl;
  return 0;
}
