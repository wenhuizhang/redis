#!/bin/bash

set -x
set -m 

rm -rf ./jemalloc
VER=5.2
git clone https://github.com/wenhuizhang/jemalloc.git
cd jemalloc 
git checkout master

./autogen.sh
rm -rf .git
make dist
make
sudo make install

cp ../../Makefile-deps Makefile
