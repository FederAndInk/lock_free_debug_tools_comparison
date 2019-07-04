#!/bin/env bash

if [ $(command -v g++) ]; then
  mkdir build_gcc
  cd build_gcc
  cmake .. -DCMAKE_CXX_COMPILER=g++
  cmake --build . -j$(nproc --all)
  cd ..

  mkdir build_gcc_O2
  cd build_gcc_O2
  cmake .. -DCMAKE_CXX_COMPILER=g++ -DOPTIMIZATION_LEVEL=2
  cmake --build . -j$(nproc --all)
  cd ..
fi
if [ $(command -v clang++) ]; then
  mkdir build_clang
  cd build_clang
  cmake .. -DCMAKE_CXX_COMPILER=clang++
  cmake --build . -j$(nproc --all)
  cd ..

  printf "#include <ciso646>\nint main () {}" | clang -E -stdlib=libc++ -x c++ -dM - | grep _LIBCPP_VERSION
  if (($? == 0)); then
    mkdir build_clang_libcxx
    cd build_clang_libcxx
    cmake .. -DCMAKE_CXX_COMPILER=clang++ -DUSE_LIBCXX=ON
    cmake --build . -j$(nproc --all)
    cd ..
  fi
fi
