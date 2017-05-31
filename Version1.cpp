// estimate pi using monte carlo methods
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <time.h>
using namespace std;

int main()
{
  // gets user input for number of trials
  cout << "Enter number of trials: " << endl;
  int nTrials; // number of trials
  cin >> nTrials;

  // set seed for random number generator
  srand(time(0));

  // generate and classify random points
  int results = 0; // number of hits in circle
  for (int i = 0; i != nTrials; ++i) {
    double x = (double)rand() / (double)RAND_MAX; // x coord
    double y = (double)rand() / (double)RAND_MAX; // y coord
    double distance = sqrt(pow(x, 2) + pow(y, 2)); // distance from origin
    if (distance <= 1) {
      ++results;
    }
  }
  // estimate area of quarter circle
  double quarterCircleArea = (double)results / (double)nTrials;

  // calculate and display estimation of pi
  double piEstimate = quarterCircleArea * 4;
  cout << "Estimate of pi = " << piEstimate << endl;

  return 0;
}
	
	
