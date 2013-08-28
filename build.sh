#!/bin/sh

cd `dirname $0`/build

cmake ..
cmake --build .

