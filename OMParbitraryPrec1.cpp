// estimate value of pi to arbitrary precision using monte carlo methods
// parallelized with OpenMP
// compile with: g++ OMParbitraryPrec1.cpp -o OMPap1 -I/path/to/gmp-6.1.2 -L/path/to/libgmpxx.la_libgmp.la -lgmpxx -lgmp -fopenmp -std=c++11
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <time.h>
#include <omp.h>
#include <gmp.h>
#include <gmpxx.h>

int main()
{
  auto start = std::chrono::steady_clock::now();  // get time at beginning of execution
  int numThreads = 16;  // number of threads for parallel section
  mpf_set_default_prec(128);  // set precision for estimate
  mpz_class numTrials = 100000000; // set number of trials
  double x,y;  // random x and y coordinates to be generated
  mpf_class numHits = 0;  // number of points inside of quarter circle
  mpf_class privateNumHits;  // private variable for parallel section
  mpz_class i;  // for loop counter
  unsigned int seed = time(0);  // seed for random number generator
  mpf_class quarterCircleArea;  // estimate for area of quarter circle
  mpf_class piEstimate;  // estimate for pi
  #pragma omp parallel num_threads(numThreads)
    mpf_class privateNumHits = 0;
    #pragma omp for private(x,y,privateNumHits) nowait
      for (i = 0; i < numTrials; ++i) {
        x = rand_r(&seed) / double(RAND_MAX);  // generate random x coordinate 0 <= x <= 1
        y = rand_r(&seed) / double(RAND_MAX);  // generate random y coordinate 0 <= y <= 1
        if (x * x + y * y <= 1) {
          privateNumHits += 1;  // if point inside circle, increment numHits
        }
      }
    #pragma omp critical
      numHits += privateNumHits;
  quarterCircleArea = numHits / numTrials; // estimates area of quarter circle
  piEstimate = quarterCircleArea * 4; // calculates estimate of pi
  auto end = std::chrono::steady_clock::now();  // get time at end of program
  std::chrono::duration<double> execTime = end - start;  // calculate execution time
  std::cout << execTime.count() << std::endl;
  return 0;
}
