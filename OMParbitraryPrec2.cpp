// estimate value of pi to arbitrary precision using monte carlo methods
// parallelized with OpenMP
// compile with: g++ OMParbitraryPrec2.cpp -o OMPap2 -I/home/jgoncal/git/gmp-6.1.2 -L/home/jgoncal/git/gmp-6.1.2 -lgmpxx -lgmp -fopenmp
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <omp.h>
#include <gmp.h>

using namespace std;

int main()
{
  int numThreads = 16;  // number of threads for parallel section
  mpf_set_default_prec(128);  // set precision for estimate
  mpf_t numTrialsMP; // multiple precision float type for number of trials
  mpf_init(numTrialsMP);
  mpf_set_ui(numTrialsMP, 100000000 * numThreads);  // set number of trials
  unsigned long long numTrials = mpf_get_ui(numTrialsMP) / numThreads;  // long long type for number of trials
  double x,y;  // random x and y coordinates to be generated
  unsigned long long numHits = 0;  // number of points inside of quarter circle
  unsigned long long i,j;  // for loop counters
  unsigned int seed;  // seed for random number generator
  mpf_t quarterCircleArea;  // estimate for area of quarter circle
  mpf_init(quarterCircleArea);
  mpf_t piEstimate;  // estimate for pi
  mpf_init(piEstimate);
  #pragma omp parallel for num_threads(numThreads) private(j,x,y) reduction(+:numHits)
  for (i = 0; i < numThreads; ++i) {
    seed = omp_get_thread_num();  // sets different seed for each thread
    for (j = 0; j < numTrials; ++j) {
      x = rand_r(&seed) / double(RAND_MAX);  // generate random x coordinate 0 <= x <= 1
      y = rand_r(&seed) / double(RAND_MAX);  // generate random y coordinate 0 <= y <= 1
      if (x * x + y * y <= 1) {
        ++numHits;  // if point inside circle, increment numHits
      }
    }
  }
  mpf_t numHitsMP; // multiple precision float type for numHits
  mpf_init(numHitsMP);
  mpf_set_ui(numHitsMP, numHits); // give numHits multiple precision
  mpf_div(quarterCircleArea, numHitsMP, numTrialsMP); // estimates area of quarter circle
  mpf_mul_ui(piEstimate, quarterCircleArea, 4); // calculates estimate of pi
  cout << "Estimate of pi: ";
  mpf_out_str(stdout, 10, 0, piEstimate); // display estimate of pi
  cout << endl;
  return 0;
}
