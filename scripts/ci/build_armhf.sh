#!/bin/bash
set -e

export PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig:/usr/arm-linux-gnueabihf/pkgconfig
export PKG_CONFIG_PATH="$HOME/cunit-install/armhf/lib/pkgconfig:${PKG_CONFIG_PATH}"

if [ "${CC#clang}" != "${CC}" ] ; then
	export CC="clang --target=arm-linux-gnueabihf"
	export LD="clang --target=arm-linux-gnueabihf"
	export CXX="clang++ --target=arm-linux-gnueabihf"
	export CFLAGS="-march=armv7-a"
else
	export CC="arm-linux-gnueabihf-gcc"
	export LD="arm-linux-gnueabihf-ld"
	export AR="arm-linux-gnueabihf-ar"
	export CXX="arm-linux-gnueabihf-g++"
fi

cd ~
export CROSS_ARCH="armhf"
#export DPDK_CROSS=arm-linux-gnueabihf
#export TARGET="arm-linux-gnueabihf$DPDKCC"


dpkg -i --force-depends ~/download/libpcap0.8-dev_1.5.3-2_armhf.deb

#./build_dpdk.sh
#DPDKPATH=`cat /tmp/dpdk_install_dir`


git clone /odp
cd ./odp
./bootstrap
./configure --host=arm-linux-gnueabihf --build=x86_64-linux-gnu \
	--disable-test-cpp ${CONF}
make clean
make -j 8
cd ..
rm -rf odp
