#!/bin/bash

#SBATCH --job-name=ex_mpi       # job's name
#SBATCH --output=outputfile-%j.out # --output = name of the output file  --error= name of error file (%j = jobID )
#SBATCH --error=errorfile-%j.err
#SBATCH -N 1                        # number of nodes (or --nodes=1)
#SBATCH -n 2                        # number of tasks (or --tasks=8)
#SBATCH --mem-per-cpu=4G            # memory per core
#SBATCH --time=00:10:00             # Wall Time 10mn
#SBATCH --account=formation_isae       # MANDATORY : account  ( launch myaccounts to list your accounts)
#SBATCH --qos=cpu_2019_80
#SBATCH --partition=cpu19_rh8
#SBATCH --export=none               # To start the job with a clean environnement and source of ~/.bashrc


# to go to the submit directory
cd ${SLURM_SUBMIT_DIR}

module load openmpi

export OMPI_MCA_btl_vader_single_copy_mechanism=none

srun --export=ALL ./test
