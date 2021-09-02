#!/bin/bash

if [ -z $1 ]
then
./blackpill.sh clean
fi

./blackpill.sh LOADER=1 $1
