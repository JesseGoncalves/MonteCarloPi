cd HPXdistributed/build/

cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/pi.cmake -DHPX_DIR=/usr/local/pi/lib/cmake/HPX ../

make

scp HPXdistributed pi@130.39.92.59:/home/pi
