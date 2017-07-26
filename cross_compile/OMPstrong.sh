#!/bin/bash

cd OMPstrongScaling/build

cmake -DCMAKE_TOOLCHAIN_FILE:FILEPATH=$HOME/raspberrypi/pi.cmake ../

make

scp OMPstrong pi@130.39.95.52:/home/pi/cross_compile

ssh pi@130.39.95.52 './OMPstrongScale.sh'
