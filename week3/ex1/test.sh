#!/bin/bash

ExecutableFile='./PlusMinus'

make # compile program with makefile

$ExecutableFile > output.txt # Run program and save output as output.txt

diff answer.txt output.txt > result.txt # compare answer.txt and output.txt with diff command as result.txt

make clean # clean object files and executable file
