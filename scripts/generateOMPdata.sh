#!/bin/bash

OMP_PLACES=cores
OMP_PROC_BIND=close

g++ ../open_mp_code/OMPstrongScale.cpp -o ompStrong -fopenmp -std=c++11
g++ ../open_mp_code/OMPweakScale.cpp -o ompWeak -fopenmp -std=c++11

./ompStrong
./ompWeak

mv OMPstrongScaling.csv ../data
mv OMPweakScaling.csv ../data
