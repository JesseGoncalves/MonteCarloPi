#!/bin/bash

module load gcc/6.3.0 cmake/3.8.1 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/build/hpx_build_Release/lib:/opt/boost/1.63.0-gcc6.3.0/release/lib:/opt/mn/gcc/6.3.0/lib64/
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/build/hpx_build_Release/lib/pkgconfig

c++ -o $HOME/repos/MonteCarloPi/executables/hpxp $HOME/repos/MonteCarloPi/hpx_code/HPXparallel1.cpp `pkg-config --cflags --libs hpx_application` -DHPX_APPLICATION_NAME=hpxp

for j in 1
do
  P=$((100000000 / $j))
  srun -p carson -N 1 --time=4:00:00 $HOME/repos/MonteCarloPi/executables/hpxp -t 10 --hpx:numa-sensitive --cores 10 --trials $j --packets $P  #run code for different numbers of processor
done

now=$(date +"%Y-%m-%d-%S")

mv HPXparData.csv $HOME/repos/MonteCarloPi/data/HPXparData_$now.csv
