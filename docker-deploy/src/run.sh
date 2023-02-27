#!/bin/bash

make clean
make 
echo 'start running proxy server...'
./proxyyy &
while true ; do continue ; done
