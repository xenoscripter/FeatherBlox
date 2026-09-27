#!/bin/sh
set -eu
mkdir -p build
cc -std=c11 -O2 -Wall -Wextra \
  $(sdl2-config --cflags) \
  src/main.c \
  -o build/featherblox \
  $(sdl2-config --libs)
echo "Built: build/featherblox"
