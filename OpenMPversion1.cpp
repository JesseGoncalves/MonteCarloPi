// estimate pi using monte carlo methods, parallelized w/ OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<omp.h>

using namespace std;

int main()
{
  int nthreads;

  // gets user input for number of trials
  cout << "Enter number of trials: " << endl;
  int nTrials; // total number of trials
  cin >> nTrials;

  // generate and classify random points in parallel
  int results = 0; // hits inside circle
  double x; // x coord
  double y; // y coord
  double distance; // distance from center of circle
#pragma omp parallel for private(x,y,distance,nthreads) shared(results)
  for (int i = 0; i < nTrials; ++i) {

    // print number of threads
    nthreads = omp_get_num_threads();
    cout << "number of threads: " << nthreads << endl;

    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;
    distance = sqrt(pow(x, 2) + pow(y, 2));
    if (distance <= 1) {
      ++results;
    }
  }

  // estimate area of quarter circle
  double qcircA = (double)results / (double)nTrials;

  // calculate and display estimation of pi
  double mcPi = qcircA * 4;
  cout << "Estimate of pi = " << mcPi << endl;

  return 0;
}
