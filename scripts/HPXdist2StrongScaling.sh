#!/bin/bash

module load gcc/6.3.0 cmake/3.6.2 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

export LD_LIBRARY_PATH=$HOME/build/hpx_build_Release/lib:/opt/boost/1.63.0-gcc6.3.0/release/lib:/opt/mn/gcc/6.3.0/lib64/
export PKG_CONFIG_PATH=$HOME/build/hpx_build_Release/lib/pkgconfig

c++ -o $HOME/repos/MonteCarloPi/executables/hpxd2 $HOME/repos/MonteCarloPi/hpx_code/HPXdistributed2.cpp `pkg-config --cflags --libs hpx_application` -DHPX_APPLICATION_NAME=hpxd2

for i in {1..14}
do
  $HOME/repos/MonteCarloPi/executables/hpxd2 -t $i --cores $i --threads 100000 --trials 1000  #run code for different numbers of processors
done

mv $HOME/repos/MonteCarloPi/executables/HPXdist2Data.csv $HOME/repos/MonteCarloPi/data
