#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/include/parallel_for_loop.hpp>
#include <hpx/lcos/gather.hpp>

#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <random>
#include <thread>
#include <vector>

boost::uint64_t trials(boost::uint64_t numTrials,
   boost::uint64_t
       packetsPerNode);    // forward declaration of trials function
HPX_PLAIN_ACTION(trials, trial_action);    // wrap function as action
char const* gather_basename =
   "/HPXdistStrongScaling/gather/";    // basename for gather operation

double rand01(unsigned int seed)    // generates a random double between 0 and 1
{
//   unsigned int seed = 0;
//      std::chrono::steady_clock::now().time_since_epoch().count() +
//      hpx::get_locality_id();

   static thread_local std::mt19937 generator(seed);

   std::uniform_real_distribution<double> distribution(0, 1);

   return distribution(generator);
}

boost::uint64_t trials(boost::uint64_t numTrials,
   boost::uint64_t packetsPerNode)    // calculates estimate of pi
{
   boost::uint64_t numHits = 0;    // number of hits inside circle

   unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count() +
      hpx::get_locality_id();

   hpx::parallel::for_loop(hpx::parallel::par, 0, packetsPerNode,
       hpx::parallel::reduction_plus(numHits),
       [&](boost::uint64_t k, boost::uint64_t& piEstimate) {

           for (boost::uint64_t i = 0; i < numTrials; ++i)
           {
               double x = rand01(seed);
               double y = rand01(seed);    // generate random x and y coordinates

               if (x * x + y * y < 1)
               {
                   numHits +=
                       1;    // increment numHits if point is inside circle
               }
           }

          // piEstimate += (4 * numHits) / (double) (numTrials * packetsPerNode);

       });

   return numHits;    // return number of points inside circle
}

HPX_REGISTER_GATHER(boost::uint64_t, monte_carlo_gather);

int hpx_main(boost::program_options::variables_map& vm)
{
   boost::uint64_t numTrials =
       vm["trials"].as<boost::uint64_t>();    // number of trials per packet

   boost::uint64_t numPackets =
       vm["packets"]
           .as<boost::uint64_t>();    // number of packets to be executed

   boost::uint64_t numNodes = hpx::get_num_localities(hpx::launch::sync);

   boost::uint64_t packetsPerNode = boost::uint64_t(numPackets /
       (double) numNodes);    // number of packets to execute on each node

   trial_action runTrials;    // call action

   //std::vector<double> estimateVec;

   double overall_time = 0;

   boost::uint64_t total_hits = 0;

   boost::uint64_t total_trials = 0;

   std::streamsize ss = std::cout.precision();

   for (int j = 0; j >= 0; ++j) {  // repeat infinitely

       auto start = std::chrono::steady_clock::
           now();    // time at beginning of execution

       hpx::future<boost::uint64_t> hits = hpx::async(runTrials, hpx::find_here(),
           numTrials, packetsPerNode);    // estimate of pi from each thread

       if (hpx::get_locality_id() != 0)
       {
           hpx::lcos::gather_there(gather_basename, std::move(hits), j);
           // pass estimate to node 0
       }
       else
       {
           hpx::future<std::vector<boost::uint64_t>> resultFuture =
               hpx::lcos::gather_here(gather_basename, std::move(hits),
                   numNodes, j);    // receive estimates from other nodes

           std::vector<boost::uint64_t> resultVec =
               resultFuture.get();    // get futures

           //double result = 0;

           for (boost::uint64_t l = 0; l < numNodes; ++l)
           {
               total_hits += resultVec[l];    // calculate overall estimate
           }

           //result = result / numNodes;

           total_trials += numTrials * numPackets;

           //estimateVec.push_back(result);

           //double overall_estimate = 0;

           //for (int k = 0; k < estimateVec.size(); ++k) {

           //    overall_estimate += estimateVec[k];

           //}

           //overall_estimate = overall_estimate / estimateVec.size();

           auto end =
               std::chrono::steady_clock::now();    // time at end of execution

           std::chrono::duration<double> execTime =
               end - start;    // execution time

           overall_time += execTime.count();

           std::cout << "(4 * " << total_hits << ") / " << total_trials << std::endl;

           //std::cout << "Estimate = " << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
              // << overall_estimate  << std::setprecision(ss) << ", Time = " << overall_time <<
              // ", Serial Time = " << overall_time*16 << std::endl;
      
        }

   }
   return hpx::finalize();
}

int main(int argc, char* argv[])
{
   boost::program_options::options_description desc("Allowed options");

   desc.add_options()
       ("packets",
           boost::program_options::value<boost::uint64_t>()->default_value(
               10000),
           "number of packets")
       ("trials",
           boost::program_options::value<boost::uint64_t>()
               ->default_value(10000),
           "number of trials");

   return hpx::init(desc, argc, argv);
}
