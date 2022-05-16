#!/bin/bash

if [ $# -ne 1 ]
then
    echo "At least the name of source file"
    exit 1
else
    ext="${1##*.}"
    echo $ext
    if [ $ext = "c" ]
    then
	gcc -std=c99 $1 -o test -fopenmp
    elif [ $ext = "f90" ] || [ $ext = "F90" ]
    then
	#gfortran $1 -o test -fopenmp
	gfortran $1 -o test -fopenmp
    else
	echo "Wrong extension : must be c or f90 files"
    fi
    
fi


    
    
 
    
