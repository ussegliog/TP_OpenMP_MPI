#!/bin/bash

# Clean PATH variable (for jupyterhub env)
export PATH=$(echo $PATH | tr : '\n'  | grep -v /softs/rh8/conda-envs/pangeo_stable/bin | paste -s -d:)


if [ $# -ne 1 ]
then
    echo "At least the name of source file"
    exit 1
fi

mpic++ $1 -o test
