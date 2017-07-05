// estimate value of pi to arbitrary precision using monte carlo methods
// arbitrary precision library GMP
// compile with: g++ ArbitraryPrec3.cpp -o ap3 -I/path/to/gmp-6.1.2 -L/path/to/lib_w/_lgmp_lgmpxx -lgmpxx -lgmp -std=c++11
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <chrono>
#include <time.h>
#include <omp.h>
#include <gmp.h>
#include <gmpxx.h>

int main()
{
  auto start = std::chrono::steady_clock::now();  // time at beginning of program
  mpf_set_default_prec(128);  // set precision for estimate
  int i,k; mpz_class j;  // for loop counters
  mpz_class numTrials = 100000000;  // number of trials performed on each node
  int numIters = 16;  // number of runs of monte carlo method
  double x;  // random x coordinates to be generated
  double y;  // random y coordinates to be generated
  mpf_class numHits;  // number of random points inside quarter circle
  unsigned int seed;  // seed for random number generator
  int threadNum;  // number associated with each thread
  mpf_class quarterCircleArea;  // estimates of area of quarter circle
  mpf_class piEstimate;  // estimates of pi
  mpf_class mean;  // mean of estimates of pi
  std::vector<mpf_class> estimateVec(numIters, 0);  // 1 x numThreads vector to hold estimates of pi
  for (i = 0; i < numIters; ++i) {
    seed = time(0);  // sets different seed for each iteration
    numHits = 0;  // initialize numHits to 0
    for (j = 0; j < numTrials; ++j) {
      x = rand_r(&seed) / double(RAND_MAX);  // generate random x coordinate 0 <= x <= 1
      y = rand_r(&seed) / double(RAND_MAX);  // generate random y coordinate 0 <= y <= 1
      if (x * x + y * y <= 1) { 
        numHits += 1;  // if point inside circle, increment numHits
      }
    }    
    quarterCircleArea = numHits / numTrials; // estimates area of quarter circle
    piEstimate = quarterCircleArea * 4; // calculates estimate of pi
    estimateVec[i] = piEstimate;  // saves estimates in vector
  }
  //for (k = 0; k < numIters; ++k) {
  //  mean += estimateVec[k] / numIters;  // calculate mean of estimates
  //}
  //std::cout << mean << std::endl;  // display mean of estimates
  auto end = std::chrono::steady_clock::now();  // time at end of program
  std::chrono::duration<double> execTime = end - start;  // program execution time
  std::cout << execTime.count() << std::endl;
  return 0;
}
