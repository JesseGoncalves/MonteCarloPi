#include <chrono>
#include <fstream>
#include <omp.h>
#include "omprng.h"

int main()
{
  unsigned long long numTrials = 100000000;
  for (int numThreads = 1; numThreads < 17; ++numThreads) {
  double x;
  auto begin = std::chrono::steady_clock::now();
  omprng rng;
  rng.setNumThreads(numThreads);
  #pragma omp parallel for private(x)
  for (int i = 0; i < numTrials; ++i) {
    x = rng.runif();
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> execTime = end - begin;
  std::ofstream results("omprngTest.csv", std::ios::app);
  results << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
  }
  return 0;
}
