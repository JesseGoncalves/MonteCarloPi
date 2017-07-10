#!/bin/bash

module load gcc/6.3.0 cmake/3.6.2 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/build/hpx_build_Release/lib:/opt/boost/1.63.0-gcc6.3.0/release/lib:/opt/mn/gcc/6.3.0/lib64/
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/build/hpx_build_Release/lib/pkgconfig

c++ -o $HOME/repos/MonteCarloPi/executables/hpxp $HOME/repos/MonteCarloPi/hpx_code/HPXparallel1.cpp `pkg-config --cflags --libs hpx_application` -DHPX_APPLICATION_NAME=hpxp

for i in {1..32}
do
  srun -p trillian -N 1 $HOME/repos/MonteCarloPi/executables/hpxp -t $i --hpx:numa-sensitive --cores $i --trials 1000 --threads 100000  #run code for different numbers of processor
done

echo '                                 ' >> $HOME/repos/MonteCarloPi/data/HPXparData.csv

cat HPXparData.csv >> $HOME/repos/MonteCarloPi/data/HPXparData.csv

rm HPXparData.csv
