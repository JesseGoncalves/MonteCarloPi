// estimate pi using monte carlo methods, parallelized w/ OpenMP
// calculates probability of random point 0 <= x,y < 1 being
// inside a quarter of a circle of radius 1
// then multiplies result by 4 to estimate pi
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<omp.h>
#include<vector>
#include<time.h>

using namespace std;

int main()
{
  // gets user input for number of trials
  cout << "Enter number of trials: " << endl;
  int nTrials; // total number of trials
  cin >> nTrials;

  // gets user input for number of threads
  cout << "Enter number of threads: " << endl;
  int numThreads; // number of threads in parallel loop
  cin >> numThreads;

  // generate vector of random points
  srand(time(0)); // set seed for random number generator
  vector<double> randomVec;
  for (int i = 0; i < nTrials * 2; ++i) {  // loop runs in parallel
    double randNum = rand() / (double)RAND_MAX;
    randomVec.push_back(randNum);
  }

  int results = 0; // hits inside circle
  double distance; // distance from center of circle
  
  #pragma omp parallel for private(distance) reduction(+:results) num_threads(numThreads)
  for (int j = 0; j < nTrials*2 - 1; j += 2) {
  distance = sqrt(pow(randomVec[j], 2) + pow(randomVec[j+1], 2));
    if (distance <= 1) {
      ++results;
    }
  }

  // estimate area of quarter circle
  double quarterCircleArea = (double)results / (double)nTrials;

  // calculate and display estimation of pi
  double piEstimate = quarterCircleArea * 4;
  cout << "Estimate of pi = " << piEstimate << endl;

  return 0;
}
