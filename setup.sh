#!/bin/bash
mkdir -p build
cd build
cmake ..
cmake --build .
echo "Build complete! Run with ./MyProject"