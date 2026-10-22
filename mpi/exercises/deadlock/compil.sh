#!/bin/bash

# Clean PATH variable (for jupyterhub env)
export PATH=$(echo $PATH | tr : '\n'  | grep -v /work/softs/rh8/conda-envs/pangeo_20260630/.pixi/envs/default/bin | paste -s -d:)

if [ $# -ne 1 ]
then
    echo "At least the name of source file"
    exit 1
fi

mpic++ $1 -o test
