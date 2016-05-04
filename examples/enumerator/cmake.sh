#!/bin/sh

BUILD_DIR="build"

do_clean() {
	rm -rf $BUILD_DIR
}

do_build() {
	mkdir $BUILD_DIR; cd $BUILD_DIR
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	make # VERBOSE=1
	cp ../../../libwebcam/usb.ids .
}

do_install() {
	cd $BUILD_DIR
	make install
}

case $1 in
clean)
	do_clean
	;;
build)
	do_build
	;;
install)
	do_install
	;;
*)
	echo "Usage: $0 { clean | build | install }"
	exit 1
	;;
esac
