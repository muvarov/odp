#!/bin/bash
# Based on a test script from avsm/ocaml repo https://github.com/avsm/ocaml

CHROOT_DIR=/tmp/arm-chroot
VERSION=trusty
CHROOT_ARCH=armhf
ARCH=${CROSS_ARCH}

# Debian package dependencies for the host
HOST_DEPENDENCIES="debootstrap qemu-user-static binfmt-support sbuild"

# Debian package dependencies for the chrooted environment
GUEST_DEPENDENCIES="build-essential m4 sudo automake autoconf \
	libtool libssl-dev graphviz mscgen \
	libpcap-dev libconfig-dev zlib1g-dev libssl-dev \
	"

function do_mounts {
	mkdir -p /proc
	mount -t proc proc /proc
	mkdir -p /sys
	mount -t sysfs sysfs /sys
}

function do_umounts {
	umount /proc
	umount /sys
}

function setup_arm_chroot {
    # Host dependencies
    sudo apt-get install -qq -y ${HOST_DEPENDENCIES}

    # Create chrooted environment
    sudo mkdir ${CHROOT_DIR}
    sudo debootstrap --foreign --no-check-gpg --include=fakeroot,build-essential,${GUEST_DEPENDENCIES} \
        --arch=${CHROOT_ARCH} ${VERSION} ${CHROOT_DIR}
    sudo cp /usr/bin/qemu-arm-static ${CHROOT_DIR}/usr/bin/
    sudo chroot ${CHROOT_DIR} ./debootstrap/debootstrap --second-stage
    sudo sbuild-createchroot --arch=${CHROOT_ARCH} --foreign --setup-only \
        ${VERSION} ${CHROOT_DIR}

    # Create file with environment variables which will be used inside chrooted
    # environment
    echo "export ARCH=${ARCH}" > envvars.sh
    echo "export TRAVIS_BUILD_DIR=${TRAVIS_BUILD_DIR}" >> envvars.sh
    chmod a+x envvars.sh

    # Create build dir and copy travis build files to our chroot environment
    sudo mkdir -p ${CHROOT_DIR}/${TRAVIS_BUILD_DIR}
    sudo rsync -av ${TRAVIS_BUILD_DIR}/ ${CHROOT_DIR}/${TRAVIS_BUILD_DIR}/

    # Indicate chroot environment has been set up
    sudo touch ${CHROOT_DIR}/.chroot_is_done

    # Call ourselves again which will cause tests to run
    sudo chroot ${CHROOT_DIR} bash -c "cd ${TRAVIS_BUILD_DIR} && ./scripts/travis-qemu.sh"
}

if [ -e "/.chroot_is_done" ]; then
  # We are inside ARM chroot
  echo "Running inside chrooted environment"

  . ./envvars.sh
else
  if [ "${CROSS_ARCH}" = "armhf" ]; then
    # ARM test run, need to set up chrooted environment first
    echo "Setting up chrooted ARM environment"
    setup_arm_chroot
  fi
  exit 0
fi

echo "Running tests"
echo "Environment: $(uname -a)"
do_mounts

./bootstrap
./configure
make check
ret=$?
if [ $ret -ne 0 ]; then
	find . -name 'test-suite.log' -execdir grep -il "FAILED" {} \; -exec echo {} \; -exec cat {} \;
fi

do_umounts
exit $ret
