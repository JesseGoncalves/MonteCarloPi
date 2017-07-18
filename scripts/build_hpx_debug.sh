#!/bin/bash

BUILDTYPE=Debug
SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
BUILD_PATH=$HOME/build/hpx_build_$BUILDTYPE
INSTALL_PATH=$HOME/local_install_$BUILDTYPE
REPO_PATH=$HOME/hpx

module load gcc/6.3.0 cmake/3.8.1 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0 

NUM_BUILDCORES=20

if [ "$1" = "clean" ]; then
    rm -rf $BUILD_PATH 
    exit 0
fi

if [ ! -d "$BUILD_PATH" ]; then
    echo "Creating build path..."
    mkdir -p "$BUILD_PATH"
    cd "$BUILD_PATH"

    ############################################
    # MODIFY CMAKE FLAGS AS YOU WISH
    #

    cmake \
        -DCMAKE_BUILD_TYPE=$BUILDTYPE \
        -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH \
	-DHPX_WITH_PARCELPORT_MPI=true \
	-DHPX_WITH_MALLOC="jemalloc"  \
	-DHPX_WITH_THREAD_IDLE_RATES=On \
        $REPO_PATH

    #
    #
    ############################################


    # cleanup on failure
    rc=$?
    if [[ $rc != 0 ]] ; then
        cd "$SCRIPTPATH"
        rm -rf "$BUILD_PATH"
        exit $rc
    fi

fi

# the actual build command
cd "$BUILD_PATH"
nice make "$@" -k -j$NUM_BUILDCORES || exit 1
make install
