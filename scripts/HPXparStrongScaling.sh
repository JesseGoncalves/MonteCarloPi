#!/bin/bash

#module load gcc/6.3.0 cmake/3.6.2 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

export COMPILER_PATH=$COMPILER_PATH:$HOME/raspberrypi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-c++
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/raspberrypi/build/hpx_build_Release/lib:$HOME/raspberrypi/boost/lib
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/raspberrypi/build/hpx_build_Release/lib/pkgconfig

c++ -o $HOME/repos/MonteCarloPi/executables/hpxp $HOME/repos/MonteCarloPi/hpx_code/HPXparallel1.cpp `pkg-config --cflags --libs hpx_application` -DHPX_APPLICATION_NAME=hpxp

for i in {1..8}
do
  T=$((2 * $i))
  $HOME/repos/MonteCarloPi/executables/hpxp -t $T --cores $i --trials 1000 --packets 100000  #run code for different numbers of processor
done

now=$(date +"%Y-%m-%d-%S")

mv HPXparData.csv $HOME/repos/MonteCarloPi/data/HPXparData_$now.csv
