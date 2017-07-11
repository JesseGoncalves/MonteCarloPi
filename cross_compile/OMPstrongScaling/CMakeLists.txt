cmake_minimum_required (VERSION 2.8)
project (strongScale CXX)
 
# add the executable
add_executable(strongScale OMPstrongScale.cpp)

target_compile_features(strongScale PRIVATE cxx_range_for)
	
FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

install (TARGETS strongScale DESTINATION bin)
