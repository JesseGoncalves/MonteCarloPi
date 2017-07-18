#!/bin/bash

module load gcc/6.3.0 cmake/3.8.1 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

export LD_LIBRARY_PATH=$HOME/build/hpx_build_Release/lib:/opt/boost/1.63.0-gcc6.3.0/release/lib:/opt/mn/gcc/6.3.0/lib64/:/opt/mn/openmpi/1.10.5-gcc6.3.0/lib/:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$HOME/build/hpx_build_Release/lib/pkgconfig:$PKG_CONFIG_PATH

c++ -o $HOME/repos/MonteCarloPi/executables/hpxdist $HOME/repos/MonteCarloPi/hpx_code/HPXdistributed1.cpp `pkg-config --cflags --libs hpx_application` -DHPX_APPLICATION_NAME=hpxdist

if [ $? != 0 ]; then 
  echo "did not compile"
  exit 1
fi

for i in {1..8}
do
  srun -p marv_ht -N $i --time=4:00:00 $HOME/repos/MonteCarloPi/executables/hpxdist -t 32 --hpx:run-hpx-main --hpx:numa-sensitive --nodes $i --packets 1000 --trials 100000  #run code on increasing numbers of nodes
done

now=$(date +"%Y-%m-%d-%S")

mv HPXdistData.csv $HOME/repos/MonteCarloPi/data/HPXdistData_$now.csv
