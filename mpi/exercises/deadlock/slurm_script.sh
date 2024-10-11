#!/bin/bash

#SBATCH --job-name=ex_mpi       # job's name
# --output = name of the output file  --error= name of error file (%j = jobID )
#SBATCH --output=outputfile-%j.out
#SBATCH --error=errorfile-%j.err
#SBATCH -N 1                        # number of nodes (or --nodes=1)
#SBATCH -n 2                        # number of tasks (or --tasks=8)
#SBATCH --mem-per-cpu=7G            # memory per core
#SBATCH --time=00:10:00             # Wall Time 10mn
#SBATCH --account=supporthpc       # MANDATORY : account  ( launch myaccounts to list your accounts)
#SBATCH --export=none               # To start the job with a clean environnement and source of ~/.bashrc


srun ./test
