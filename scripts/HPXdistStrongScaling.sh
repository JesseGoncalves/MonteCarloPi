#!/bin/bash

module load gcc/6.3.0 cmake/3.6.2 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jgoncalves/build/hpx_build_Release/lib:/opt/boost/1.63.0-gcc6.3.0/release/lib:/opt/mn/gcc/6.3.0/lib64/
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/build/hpx_build_Release/lib/pkgconfig

./pkg-config_HPXdist1.sh  #compile hpx code with pkg-config

for i in {1..16}
do
  ./hpxd -t $i  #run code for different numbers of processors
done

mv HPXdistData.csv ../data
