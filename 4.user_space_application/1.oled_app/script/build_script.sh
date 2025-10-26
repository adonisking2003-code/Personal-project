#!/bin/bash

# Build script for the OLED application
cd ../build 
rm -rf *
source /home/adonisking/Learning_Linux/3.Device_driver/SDK/environment-setup-cortexa72-poky-linux
cmake -DCMAKE_TOOLCHAIN_FILE=$OE_CMAKE_TOOLCHAIN_FILE ..