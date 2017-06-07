// function estimates pi using monte carlo methods
// calculates the probability that a random point 0 <= x,y < 1
// lies inside a quarter of a circle with radius 1 centered at the origin
// then multiplies result by 4 to estimate pi
#include <cstdlib>
#include <cmath>

double monteCarloPi(int numTrials)
{
  int results = 0; // number of hits in circle
  for (int i = 0; i < numTrials; ++i) {
    double x = std::rand() / (double)RAND_MAX; // x coord
    double y = std::rand() / (double)RAND_MAX; // y coord
    double distance = std::sqrt(std::pow(x, 2) + std::pow(y, 2)); // distance from origin
    if (distance <= 1) {
      ++results;
    }
  }
  // estimate area of quarter circle
  double quarterCircleArea = (double)results / (double)numTrials; 

  // calculate estimation of pi
  double estimate = quarterCircleArea * 4;
  return estimate;
}
