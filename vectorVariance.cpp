// function calculates variance of elements in a vector
#include <vector>
#include <stdexcept>
#include <cmath>
#include "vectorMean.h"

double vectorVariance(std::vector<double> vec)
{ 
  double mean = 0;  // mean of elements in vector
  mean = vectorMean(vec);  // calls mean function
  double variance = 0;  // variance of elements in vector
    for (int b = 0; b < 10; ++b) {
  variance += std::pow(vec[b] - mean, 2) / 10;
  }
  return variance;
}
