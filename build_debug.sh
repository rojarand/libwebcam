#!/bin/bash

rm -rf build_release

set -e
mkdir build_release;cd build_release
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8

cd ..
./gradlew -Pnative_build_location=../build_release install