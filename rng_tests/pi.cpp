#include <iostream>
#include <iomanip>
#include <chrono>
#include "omprng.h"

using namespace std;


int main ()
{
    for (int numThreads = 1; numThreads < 17; ++numThreads) {
      for (int j = 0; j < 10; ++j) {
        auto start = chrono::steady_clock::now();
	int N=10e6;					//number of Monte Carlo iterations
	int n=0;					//number of points in circle
	double x, y;

        
	omprng myRng; 					//Create omprng object called myRng. This is the
									//only statement required to setup the RNG.

	//myRng.fixedSeed(12345);		//By default, the (random) seed is set according to the clock.
									//A fixed seed can be set using any positive integer.

	myRng.setNumThreads(numThreads);		//Manually set number of threads; default is the 
									//number of processors. Use 1 for serial execution.


	#pragma omp parallel for num_threads(numThreads) private(x,y) reduction(+: n)
	for(int i=0; i<N; i++) {
		x = myRng.runif();					//generate a random x between 0 and 1
		y = myRng.runif();					//generate a random y between 0 and 1
		if(x*x + y*y < 1.0) n = n+1;		//if point is in circle, then increment n 
	}
        auto end = chrono::steady_clock::now();
        chrono::duration<double> execTime = end - start;
	std::cout << "Monte Carlo estimate of pi based on " << N << " iterations on " << numThreads
             << " threads is " << std::setprecision(10) << 4.0*double(n)/double(N) << ", which took "
             << execTime.count() << " seconds." << std::endl;

      }
    }
	return 0 ;

}


