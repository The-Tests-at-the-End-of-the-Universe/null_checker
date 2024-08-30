#!/bin/bash

make re
cc test.c -o test
LD_PRELOAD=./zaphod.so ./test