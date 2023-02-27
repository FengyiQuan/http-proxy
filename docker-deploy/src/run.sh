#!/bin/bash

make clean
make daemon
echo 'start running proxy server...'
./proxyyy &
while true ; do continue ; done
