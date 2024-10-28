#!/bin/bash
#https://stackoverflow.com/questions/8779951/how-do-i-run-a-shell-script-without-using-sh-or-bash-commands

make -C ../ re
cc test.c -g -o test
LD_PRELOAD=../zaphod.so ./test