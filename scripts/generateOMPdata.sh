#!/bin/bash

OMP_PLACES=cores
OMP_PROC_BIND=close

g++ $HOME/repos/MonteCarloPi/open_mp_code/OMPstrongScale.cpp -o $HOME/repos/MonteCarloPi/executables/ompStrong -fopenmp -std=c++11
g++ $HOME/repos/MonteCarloPi/open_mp_code/OMPweakScale.cpp -o $HOME/repos/MonteCarloPi/executables/ompWeak -fopenmp -std=c++11

srun -p marvin -N $i ./$HOME/repos/MonteCarloPi/executables/ompStrong
srun -p marvin -N $i ./$HOME/repos/MonteCarloPi/executables/ompWeak

mv $HOME/repos/MonteCarloPi/executables/OMPstrongScaling.csv $HOME/repos/MonteCarloPi/data
mv $HOME/repos/MonteCarloPi/executables/OMPweakScaling.csv $HOME/repos/MonteCarloPi/data
