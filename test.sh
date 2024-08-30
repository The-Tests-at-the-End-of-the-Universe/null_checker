#!/bin/bash

cc test.c -o test
LD_PRELOAD=./zaphod.so ./test