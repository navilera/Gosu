#!/bin/bash

make -f build/bluepill/Makefile clean
make -f build/bluepill/Makefile $1 $2
