cmake_minimum_required (VERSION 2.6)
project (HPXparallel)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release."
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# default installation
#get_filename_component (default_prefix "../install" ABSOLUTE)
#set (CMAKE_INSTALL_PREFIX ${default_prefix} CACHE STRING
#      "Choose the installation directory; by default it installs in install."
#      FORCE)
#message("CMAKE_INSTALL_PREFIX:" ${default_prefix})
  #HPX
  cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
  find_package(HPX REQUIRED)
#  include_directories(${HPX_INCLUDE_DIRS})
#  link_directories(${HPX_LIBRARY_DIRS})
  message("HPX_INCLUDE_DIRS:" ${HPX_INCLUDE_DIRS})
  message("HPX_LIBRARY_DIR:" ${HPX_LIBRARY_DIR})

if(NOT (CMAKE_BUILD_TYPE STREQUAL HPX_BUILD_TYPE))
message(FATAL_ERROR "CMAKE_BUILD_TYPE doesn't match HPX_BUILD_TYPE (${HPX_BUILD_TYPE})")
endif()


if(PROFILE)
	set (CMAKE_CXX_FLAGS "-p -g ${CMAKE_CXX_FLAGS}")
	message("CXX Profiling flags enabled")
endif()

message("CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
message("CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})

message("CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("CMAKE_INCLUDE_DIRECTORIES:" ${CMAKE_INCLUDE_DIRECTORIES})

add_executable(
  HPXparallel
  HPXparallel1.cpp
)

target_compile_features(HPXparallel PRIVATE cxx_range_for)
target_link_libraries(HPXparallel hpx)

hpx_setup_target(
  HPXparallel
)

install (TARGETS HPXparallel DESTINATION bin)

#add_subdirectory(src)
