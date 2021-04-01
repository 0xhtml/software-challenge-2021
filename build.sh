#!/bin/sh

mkdir -p cmake
cd cmake

if [ ! -f CMakeLists.txt.sha1 ] || ! sha1sum -c CMakeLists.txt.sha1; then
    cmake ..
    sha1sum ../CMakeLists.txt > CMakeLists.txt.sha1
fi

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
