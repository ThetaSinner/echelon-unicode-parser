#!/bin/bash

# Script to set up https://github.com/google/benchmark

apt-get update
apt-get install -y git cmake
apt-get clean

mkdir -p /opt/google-benchmark && cd /opt/google-benchmark
git clone https://github.com/google/benchmark.git
git clone https://github.com/google/googletest.git benchmark/googletest

mkdir build && cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../benchmark
make
make install
