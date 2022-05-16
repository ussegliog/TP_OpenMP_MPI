#!/bin/bash

if [ $# -ne 1 ]
then
    echo "At least the name of source file"
    exit 1
fi

mpic++ $1 -o test
