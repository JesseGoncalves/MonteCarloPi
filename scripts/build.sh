#!/bin/bash

BUILD_TYPE=RelWithDebInfo
SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
REPO_PATH=${SCRIPTPATH}/..
BUILD_DIR=${REPO_PATH}/build
module purge
module load gcc/6.3.0 cmake/3.6.2 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0

if [ "$1" = "clean" ]; then
    CLEANCMD="rm -rf ${BUILD_DIR}"
    echo "about to execute:"
    echo "${CLEANCMD}"
    echo "is this okay? [y/N]"
    read answer
    if echo "$answer" | grep -iq "^y" ;then
        echo "removing build directory ${BUILD_DIR}"
        $CLEANCMD
	exit 0
    else
        echo "doing nothing."
	exit 0
    fi
fi

mkdir -p ${BUILD_DIR} || echo "${BUILD_DIR} already exists, continuing"
cd ${BUILD_DIR} || {
    echo "could not cd into build directory ${BUILD_DIR}, quitting."
    exit
}

cmake \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_PREFIX_PATH=${HOME}/work/local_install_${BUILD_TYPE} \
    -DCMAKE_C_COMPILER=/opt/mn/gcc/6.3.0/bin/gcc \
    -DCMAKE_CXX_COMPILER=/opt/mn/gcc/6.3.0/bin/g++ \
    ${REPO_PATH}

make HPXversion1
