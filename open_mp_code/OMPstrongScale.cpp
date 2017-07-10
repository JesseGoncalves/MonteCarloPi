#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>
#include <omp.h>

double rand01()  // generates random number from uniform distribution between 0 and 1
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count() + omp_get_thread_num();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}


int main(int argc, char* argv[])  // input number of threads from command line
{
  static unsigned long long numTrials = 10e6;  // number of trials for monte carlo simulation

  int numCores;  // number of threads for parallel section

  std::stringstream convert(argv[1]);  // initialize stringstream variable with the input from argv[1]

  if (!(convert >> numCores)) {  // convert agrv[1] to an integer

    numCores = 1;  // default value if conversion fails

  }

  unsigned long long trialsPerCore;  // number of trials run on each thread

  double x, y;  // random x and y coordinates

  unsigned long long numHits;  // number of points generated inside circle

  double piEstimate;  // estimate of pi

  double meanEstimate;  // mean of all estimates

  int i, j, l; unsigned long long k;  // for loop counters

  for (i = 0; i < 10; ++i) {  // repeat 10 times at each number of threads

    std::vector<double> estimateVec(numCores);  // vector for estimates of pi

    trialsPerCore = (unsigned long long)(numTrials / (double)numCores);  // set trials per thread

    meanEstimate = 0;  // reset mean estimate

    auto start = std::chrono::steady_clock::now();  // get time at beginning of execution

    #pragma omp parallel for num_threads(numCores) private(k,x,y,numHits,piEstimate)

    for (j = 0; j < numCores; ++j) {

      numHits = 0;  // reset number of hits inside circle

      for (k = 0; k < trialsPerCore; ++k) {
   
        x = rand01(); y = rand01();  // generate random x and y coordinates
 
        if (x * x + y * y < 1) {  

          numHits += 1;  // increment numHits if point is inside circle
  
        }

      }

      piEstimate = 4 * numHits / (double)trialsPerCore;  // calculate estimate of pi
                 
      estimateVec[omp_get_thread_num()] = piEstimate;  // store estimate in vector

    }
   
    for (l = 0; l < numCores; ++l) {

      meanEstimate += estimateVec[l] / numCores;

    }
    
    auto end = std::chrono::steady_clock::now();  // get time at end of execution

    std::chrono::duration<double> execTime = end - start;  // calculate execution time

    std::ofstream resultFile("OMPparStrongScaling.csv", std::ios::out | std::ios::app);

    resultFile << meanEstimate << ", " << numTrials << ", " << numCores << ", " << execTime.count() << std::endl;

  }
  
  return 0;

}
