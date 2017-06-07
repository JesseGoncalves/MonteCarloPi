// function calculates mean value of elements in a vector
#include <vector>
#include <stdexcept>

double vectorMean(std::vector<double> vec) 
{ 
  typedef std::vector<double>::size_type vec_size;
  vec_size vecSize = vec.size();  // size of vector
  if (vecSize == 0) {
    throw std::domain_error("mean of empty vector");
  }
  double mean = 0;  // mean of elements in vector
  for (int i = 0; i < vecSize; ++i) {
    mean += vec[i] / vecSize;
  }
  return mean;
}
