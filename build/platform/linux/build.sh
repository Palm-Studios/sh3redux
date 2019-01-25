#!/bin/bash

echo "--==SILENT HILL 3: Redux Windows Build Script==--"
echo "Copyright (C) 2016-2019 Palm Studios"

echo Building libraries...
cd ../../../libs
cd glew-2.1.0
cd build
cmake ./cmake 
make -j 4

cd ../../zlib-1.2.11
mkdir -p  build
cd build
cmake ../
make -j4

echo "Building SDL2"
cd ../../SDL2-2.0.9
mkdir -p build
chmod +x configure
cd build
../configure --without-x 
make -j4

cd ../../../
premake5 gmake
make

