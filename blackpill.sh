#!/bin/bash

make -f build/blackpill/Makefile clean
make -f build/blackpill/Makefile $1 $2
