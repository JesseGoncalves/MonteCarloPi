// function calculates standard deviation of elements in vector
#include <vector>
#include <cmath>
#include "vectorVariance.h"

double vectorStandardDev(std::vector<double> vec)
{
  double variance = 0;  // variance of elements in vector
  variance = vectorVariance(vec);
  double standardDev = std::pow(variance, .5);
  return standardDev;
} 
