#!/bin/sh

mkdir -p cmake
cd cmake

cmake ..

if [ -z "$1" ]; then
    TARGET="main"
else
    TARGET=$1
fi

if [ -z "$2" ]; then
    CONFIG="Release"
else
    CONFIG=$2
fi

cmake --build . --config $CONFIG --target $TARGET
