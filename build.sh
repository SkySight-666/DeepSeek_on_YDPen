#!/bin/bash
rm -rf ./out/dsProject
rm -rf cmake_install.cmake
rm -rf CMakeCache.txt
rm -rf CMakeFiles

cmake CMakeLists.txt

make



