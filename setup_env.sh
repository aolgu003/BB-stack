#!/usr/bin/env bash

apt-get update
apt-get install -y cmake
apt-get install -y build-essential
apt-get install -y libglib2.0-dev
apt-get install -y lcov

apt-get install -y gcc-arm-linux-gnueabihf
apt-get install -y g++-arm-linux-gnueabihf
apt-get install -y libc6-dev-armhf-cross
apt-get install -y qemu-user-static
apt install -y qemu-user

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$LCM_INSTALL_DIR/pkgconfig

apt-get install -y wget
wget https://github.com/zeromq/libzmq/releases/download/v4.2.2/zeromq-4.2.2.tar.gz
tar -xf zeromq-4.2.2.tar.gz
cd zeromq-4.2.2/
./configure --host=arm-linux-gnueabihf
make
make install
cd ..

wget https://github.com/zeromq/cppzmq/archive/v4.2.2.tar.gz
tar -xf v4.2.2.tar.gz
cd cppzmq-4.2.2/
mkdir build
cd build/
cmake .. -DCMAKE_CXX_COMPILER="/usr/bin/arm-linux-gnueabihf-g++" -DCMAKE_C_COMPILER="/usr/bin/arm-linux-gnueabihf-gcc"
make
make install
cd ..
