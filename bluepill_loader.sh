#!/bin/bash

if [ -z $1 ]
then
./bluepill.sh clean
fi

./bluepill.sh LOADER=1 $1
