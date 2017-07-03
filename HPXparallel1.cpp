#include <cstddef>
#include <cstdint>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <thread>

#include <hpx/hpx_main.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/parallel_for_loop.hpp>

double rand01()  // generates a random double between 0 and 1
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}

double trials(int numTrials)  // calculates estimate of pi
{ 
  int numHits = 0;  // number of hits inside circle 

  for (int i = 0; i < numTrials; ++i) {

    double x = rand01(); double y = rand01();  // generate random x and y coordinates

    if (x * x + y * y < 1) {

      numHits += 1;  // increment numHits if point is inside circle
   
    }
 
  }

  return 4 * numHits / (double) numTrials;  // return estimate of pi
}


int main(int argc, char* argv[])
{
  static int numTrials = 1000;  // number of trials to execute on each thread

  static int numThreads = 100000;  // number of threads to be executed

  double piEstimate = 0;  // estimate of pi

  auto start = std::chrono::steady_clock::now();  // time at beginning of execution

  hpx::parallel::for_loop(hpx::parallel::par, 0, numThreads, hpx::parallel::reduction_plus(piEstimate), [](int j, double& piEstimate) {

    piEstimate += trials(numTrials) / numThreads;  // sums up estimates on all threads

  });

  auto end = std::chrono::steady_clock::now();  // time at end of execution

  std::chrono::duration<double> execTime = end - start;  // execution time

  std::ofstream resultFile("HPXdata.csv", std::ios::out | std::ios::app);  // write results to .csv file

  resultFile << piEstimate << ", " << numTrials << ", " << numThreads << ", " << execTime.count() << std::endl;

  return 0;
}

