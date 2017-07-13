#include <cstddef>
#include <cstdint>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <thread>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/lcos/gather.hpp>
#include <hpx/include/parallel_for_loop.hpp>

double trials(boost::uint64_t numTrials, boost::uint64_t packetsPerNode);  // forward declaration of trials function
HPX_PLAIN_ACTION(trials, trial_action);  // wrap function as action
char const* gather_basename = "/HPXdistStrongScaling.cpp/gather/";  // basename for gather operation

double rand01()  // generates a random double between 0 and 1
{
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count() + hpx::get_locality_id();

  static thread_local std::mt19937 generator(seed);

  std::uniform_real_distribution<double> distribution(0, 1);

  return distribution(generator);
}

double trials(boost::uint64_t numTrials, boost::uint64_t packetsPerNode)  // calculates estimate of pi
{ 
  double piEstimate = 0;  // reset estimate of pi
 
  hpx::parallel::for_loop(hpx::parallel::par, 0, packetsPerNode, hpx::parallel::reduction_plus(piEstimate), [&](boost::uint64_t k, double& piEstimate) {

    boost::uint64_t numHits = 0;  // number of hits inside circle 

    for (boost::uint64_t i = 0; i < numTrials; ++i) {

      double x = rand01(); double y = rand01();  // generate random x and y coordinates

      if (x * x + y * y < 1) {

        numHits += 1;  // increment numHits if point is inside circle
   
      }
 
    }

    piEstimate += (4 * numHits) / (double)(numTrials * packetsPerNode);  

  });

  return piEstimate;  // return estimate of pi
}

int hpx_main(boost::program_options::variables_map& vm)
{
  boost::uint64_t numTrials = vm["trials"].as<boost::uint64_t>();  // number of trials per packet

  boost::uint64_t numPackets = vm["packets"].as<boost::uint64_t>();  // number of packets to be executed

  boost::uint64_t numNodes = vm["nodes"].as<boost::uint64_t>();  // number of nodes to execute on

  boost::uint64_t packetsPerNode = boost::uint64_t(numPackets / (double)numNodes);  // number of packets to execute on each node

  trial_action runTrials;  // call action

  for (int j = 0; j < 10; ++j) {  // repeat 10 times

    auto start = std::chrono::steady_clock::now();  // time at beginning of execution

    hpx::future<double> piEstimate = hpx::async(runTrials, hpx::find_here(), numTrials, packetsPerNode);  // estimate of pi from each thread

    if (hpx::get_locality_id() != 0) {

      hpx::lcos::gather_there(gather_basename, std::move(piEstimate)).wait();  // pass estimate to node 0

    } else {

      hpx::future<std::vector<double>> resultFuture = hpx::lcos::gather_here(gather_basename, std::move(piEstimate), numNodes);  // receive estimates from other nodes

      std::vector<double> resultVec = resultFuture.get();  // get futures

      double result = 0;

      for (boost::uint64_t l = 0; l < numNodes; ++l) {

        result += resultVec[l];  // calculate overall estimate

      }

      auto end = std::chrono::steady_clock::now();  // time at end of execution

      std::chrono::duration<double> execTime = end - start;  // execution time

      std::ofstream resultFile("HPXdistData.csv", std::ios::out | std::ios::app);  // write results to .csv file

      resultFile << result << ", " << numTrials << ", " << numPackets << ", " << numNodes << ", " << execTime.count() << std::endl;

    }

  }
  return hpx::finalize();
}

int main(int argc, char* argv[])
{
  boost::program_options::options_description desc("Allowed options");

  desc.add_options()

    ("nodes", boost::program_options::value<boost::uint64_t>()->default_value(1), "number of nodes")

    ("packets", boost::program_options::value<boost::uint64_t>()->default_value(100000), "number of packets")

    ("trials", boost::program_options::value<boost::uint64_t>()->default_value(1000), "number of trials");

  return hpx::init(desc, argc, argv);
}

