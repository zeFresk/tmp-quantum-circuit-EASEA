#!/bin/bash

if [[ "$EZ_PATH" == "" ]]; then
	echo "ERROR: Please define EZ_PATH"
	exit 1
fi

echo "Updating git submodules..."
git submodule foreach git pull

echo "Running easena..."
$EZ_PATH/bin/easena to_gates.ez

echo "Overriding CMakeLists..."
cp CMakeLists.txt.bk CMakeLists.txt

echo "Configuring CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug .

echo "Building executable..."
cmake --build .

echo "done!"
