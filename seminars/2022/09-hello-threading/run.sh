#!/usr/bin/env bash

set -exu

COMPILER="${CC:-clang++}"
EXECUTABLE=${1/.cpp/}

$COMPILER $1 -o $EXECUTABLE -std=c++20 -O3 -pthread -ggdb
./$EXECUTABLE
