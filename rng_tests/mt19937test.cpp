#include <random>
#include <thread>
#include <chrono>
#include <fstream>
#include <omp.h>

double rand01()
{
  int seed = std::chrono::steady_clock::now().time_since_epoch().count() + omp_get_thread_num();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}

int main()
{
  unsigned long long numTrials = 100000000;
  
  double x;

  for (int numThreads = 1; numThreads < 17; ++numThreads) {

    auto begin = std::chrono::steady_clock::now();

    #pragma omp parallel for num_threads(numThreads) private(x)
    for (int i = 0; i < numTrials; ++i) {
  
      x = rand01();
  
    }
  
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> execTime = end - begin;

    std::ofstream results("mt19937test.csv", std::ios::app);
 
    results << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;

  } 
  
  return 0;

}
