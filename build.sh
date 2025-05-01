#!/usr/bin/env bash
# Builds watchy.

build_dir="bin"
mkdir -p $build_dir

cd $build_dir
rm -rf ./*
cmake ../ && make
