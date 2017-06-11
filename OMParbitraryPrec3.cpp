// estimate value of pi to arbitrary precision using monte carlo methods
// parallelized with OpenMP, arbitrary precision library GMP
// compile with: g++ OMParbitraryPrec3.cpp -o OMPap3 -I/path/to/gmp-6.1.2 -L/path/to/lib_w/_lgmp_lgmpxx -lgmpxx -lgmp -fopenmp
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <omp.h>
#include <gmp.h>
#include <gmpxx.h>

int main()
{
  mpf_set_default_prec(128);  // set precision for estimate
  int numThreads = 16;  // number of threads for parallel section
  int i,k; mpz_class j;  // for loop counters
  mpz_class numTrials = 100000000;  // number of trials performed on each node
  double x;  // random x coordinates to be generated
  double y;  // random y coordinates to be generated
  mpz_class numHits = 0;  // number of random points inside quarter circle
  unsigned int seed;  // seed for random number generator
  mpf_class quarterCircleArea;  // estimates of area of quarter circle
  mpf_class piEstimate;  // estimates of pi
  mpf_class mean;  // mean of estimates of pi
  std::vector<mpf_class> estimateVec(numThreads, 0);  // 1 x numThreads vector to hold estimates of pi
  #pragma omp parallel for num_threads(numThreads) private(seed,i,j,x,y,numHits,quarterCircleArea,piEstimate)
  for (i = 0; i < numThreads; ++i) {
    seed = omp_get_thread_num();  // sets different seed for each thread
    for (j = 0; j < numTrials; ++j) {
      x = rand_r(&seed) / double(RAND_MAX);  // generate random x coordinate 0 <= x <= 1
      y = rand_r(&seed) / double(RAND_MAX);  // generate random y coordinate 0 <= y <= 1
      if (x * x + y * y <= 1) { 
        ++numHits;  // if point inside circle, increment numHits
      }
    }    
    quarterCircleArea = numHits / numTrials; // estimates area of quarter circle
    piEstimate = quarterCircleArea * 4; // calculates estimate of pi
    estimateVec[seed] = piEstimate;  // saves estimates in vector
  }
  for (k = 0; k < numThreads; ++k) {
    mean += estimateVec[k] / numThreads;  // calculate mean of estimates
  }
  std::cout << mean << std::endl;  // display mean of estimates
  return 0;
}