#!/bin/bash
set -x
set -m

wget http://download.redis.io/releases/redis-5.0.5.tar.gz
tar xzf redis-5.0.5.tar.gz
rm redis-5.0.5.tar.gz
rm redis-5.0.5/deps/update-jemalloc.sh
cp update-jemalloc.sh redis-5.0.5/deps/update-jemalloc.sh
cd redis-5.0.5/deps/
./update-jemalloc.sh

