#!/usr/bin/env bash

git submodule update --init --recursive

./compile.sh Release PROD
./copy.sh
