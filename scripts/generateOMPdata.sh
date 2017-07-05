#!/bin/bash

OMP_PLACES=cores
OMP_PROC_BIND=close

g++ ../open_mp_code/OMPstrongScale.cpp -o ../executables/ompStrong -fopenmp -std=c++11
g++ ../open_mp_code/OMPweakScale.cpp -o ../executables/ompWeak -fopenmp -std=c++11

./../executables/ompStrong
./../executables/ompWeak

mv ../executables/OMPstrongScaling.csv ../data
mv ../executables/OMPweakScaling.csv ../data
