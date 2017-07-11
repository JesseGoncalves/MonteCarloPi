cd HPXparallel/build/

cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/pi.cmake -DHPX_DIR=/usr/local/pi/lib/cmake/HPX ../

make

scp HPXparallel pi@130.39.95.52:/home/pi/cross_compile

ssh pi@130.39.95.52 './HPXparStrongScaling.sh'
