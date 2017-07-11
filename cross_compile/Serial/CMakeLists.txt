cmake_minimum_required (VERSION 2.8)
project (serialMonteCarlo CXX)
 
# add the executable
add_executable(serialMonteCarlo serialMonteCarlo.cpp)

target_compile_features(serialMonteCarlo PRIVATE cxx_range_for)

install (TARGETS serialMonteCarlo DESTINATION bin)
