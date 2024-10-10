#!/bin/bash

make re
cc test.c -g -o test
LD_PRELOAD=./zaphod.so ./test