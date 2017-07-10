#!/bin/bash

OMP_PLACES=cores
OMP_PROC_BIND=close

g++ $HOME/repos/MonteCarloPi/open_mp_code/OMPstrongScale.cpp -o $HOME/repos/MonteCarloPi/executables/OMPstrong -fopenmp -std=c++11
g++ $HOME/repos/MonteCarloPi/open_mp_code/OMPweakScale.cpp -o $HOME/repos/MonteCarloPi/executables/OMPweak -fopenmp -std=c++11

for i in {1...8}
do
  srun -p marvin -N 1 $HOME/repos/MonteCarloPi/executables/OMPstrong $i
  srun -p marvin -N 1 $HOME/repos/MonteCarloPi/executables/OMPweak $i
done

cat OMPparStrongScaling.csv >> $HOME/repos/MonteCarloPi/data/OMPparStrongScaling.csv
rm OMPparStrongScaling.csv
cat OMPparWeakScaling.csv >> $HOME/repos/MonteCarloPi/data/OMPparWeakScaling.csv
rm OMPparWeakScaling.csv
