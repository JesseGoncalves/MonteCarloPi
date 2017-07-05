#include <cstdlib>
#include <chrono>
#include <fstream>
#include <omp.h>

int main()
{
  unsigned long long numTrials = 100000000;
  double x; unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();
  for (int numThreads = 1; numThreads < 17; ++numThreads) {
  auto begin = std::chrono::steady_clock::now();
  #pragma omp parallel for num_threads(numThreads) private(x)
  for (int i = 0; i < numTrials; ++i) {
    x = rand_r(&seed) / (double)RAND_MAX;
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> execTime = end - begin;
  std::ofstream results("rand_rTest.csv", std::ios::app);
  results << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;
  }
  return 0;
}
