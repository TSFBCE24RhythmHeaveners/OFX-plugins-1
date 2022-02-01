#!/bin/bash

PLUGIN_NAME="rgb"
CPP_SOURCE_NAME="ims"

mkdir build
cd build

# make build files
cmake ..
#  cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..

# build binaries
cmake --build . --config Release

# set to ofxplugs dir
mkdir -p ~/Projects/OFX/Plugins/$PLUGIN_NAME.ofx.bundle/Contents/Linux-x86-64
mv $CPP_SOURCE_NAME.ofx ~/Projects/OFX/Plugins/$PLUGIN_NAME.ofx.bundle/Contents/Linux-x86-64/$PLUGIN_NAME.ofx

# clean
cd ..
rm -rf build

# load plugin for testing
natron dev-misc/natron-test-linux.ntp
