#include <cstddef>
#include <cstdint>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <thread>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/parallel_for_loop.hpp>

double rand01()  // generates a random double between 0 and 1
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}

double trials(boost::uint64_t numTrials)  // calculates estimate of pi
{ 
  boost::uint64_t numHits = 0;  // number of hits inside circle 

  for (boost::uint64_t i = 0; i < numTrials; ++i) {

    double x = rand01(); double y = rand01();  // generate random x and y coordinates

    if (x * x + y * y < 1) {

      numHits += 1;  // increment numHits if point is inside circle
   
    }
 
  }

  return 4 * numHits / (double) numTrials;  // return estimate of pi
}


int hpx_main(boost::program_options::variables_map& vm)
{
  boost::uint64_t numTrials = vm["trials"].as<boost::uint64_t>();  // number of trials in each packet

  boost::uint64_t numPackets = vm["packets"].as<boost::uint64_t>();;  // number of packets to be executed

  boost::uint64_t numCores = vm["cores"].as<boost::uint64_t>();  // number of cores to execute on

  for (int j = 0; j < 10; ++j) {  // repeat 10 times

    double piEstimate = 0;  // estimate of pi

    auto start = std::chrono::steady_clock::now();  // time at beginning of execution

    hpx::parallel::for_loop(hpx::parallel::par, 0, numPackets, hpx::parallel::reduction_plus(piEstimate), [&](int k, double& piEstimate) {

      piEstimate += trials(numTrials) / numPackets;  // sums up estimates on all threads

    });

    auto end = std::chrono::steady_clock::now();  // time at end of execution
 
    std::chrono::duration<double> execTime = end - start;  // execution time

    std::ofstream resultFile("HPXparData.csv", std::ios::out | std::ios::app);  // write results to .csv file

    resultFile << piEstimate << ", " << numTrials << ", " << numPackets << ", " << numCores << ", " << execTime.count() << std::endl;

  }
  return hpx::finalize();
}

int main(int argc, char* argv[])
{
  boost::program_options::options_description desc("Allowed options");

  desc.add_options()

    ("trials", boost::program_options::value<boost::uint64_t>()->default_value(1000), "number of trials per packet")
  
    ("packets", boost::program_options::value<boost::uint64_t>()->default_value(100000), "number of packets")

    ("cores", boost::program_options::value<boost::uint64_t>()->default_value(1), "number of cores");

  return hpx::init(desc, argc, argv);
}
