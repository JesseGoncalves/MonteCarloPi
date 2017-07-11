#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>
#include <omp.h>

double rand01()
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count() + omp_get_thread_num();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}


int main(int argc, char* argv[])
{
  static unsigned long long numTrials = 10e8;  // number of trials for monte carlo simulation

  int numCores;  // number of threads for parallel section

  std::stringstream convert(argv[1]);  // initialize sstream object to argv[1]

  if (!(convert >> numCores)) {  // convert argv[1] to int

    numCores = 1;  // default value if conversion fails

  }

  double x, y;  // random x and y coordinates

  unsigned long long numHits;  // number of points generated inside circle

  double piEstimate;  // estimate of pi

  double meanEstimate;  // mean of all estimates

  int i, j, l; unsigned long long k;  // for loop counters

  std::vector<double> estimateVec(numCores);  // vector for estimates of pi

  for (i = 0; i < 10; ++i) {  // repeat 10 times

    meanEstimate = 0;  // reset mean estimate

    auto start = std::chrono::steady_clock::now();  // get time at beginning of execution

    #pragma omp parallel for num_threads(numCores) private(k,x,y,numHits,piEstimate)

    for (j = 0; j < numCores; ++j) {

      numHits = 0;  // reset number of hits inside circle

      for (k = 0; k < numTrials; ++k) {
   
        x = rand01(); y = rand01();  // generate random x and y coordinates
 
        if (x * x + y * y < 1) {  

          numHits += 1;  // increment numHits if point is inside circle
  
        }

      }

      piEstimate = 4 * numHits / (double)numTrials;  // calculate estimate of pi
                 
      estimateVec[omp_get_thread_num()] = piEstimate;  // store estimate in vector

    }
   
    for (l = 0; l < numCores; ++l) {

      meanEstimate += estimateVec[l] / numCores;

    }
    
    auto end = std::chrono::steady_clock::now();  // get time at end of execution

    std::chrono::duration<double> execTime = end - start;  // calculate execution time

    std::ofstream resultFile("OMPparWeakScaling.csv", std::ios::out | std::ios::app);

    resultFile << meanEstimate << ", " << numTrials << ", " << numCores << ", " << execTime.count() << std::endl;

  }

  return 0;
} 
