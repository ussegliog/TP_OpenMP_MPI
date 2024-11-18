#!/bin/bash

#SBATCH --job-name=ex_mpi       # job's name
# --output = name of the output file  --error= name of error file (%j = jobID )
#SBATCH --output=outputfile-%j.out
#SBATCH --error=errorfile-%j.err
#SBATCH -N 2                        # number of nodes (or --nodes=1)
#SBATCH --ntasks-per-node=2 # MPI processus per node
#SBATCH --ntasks-per-core=1 # Nb of threads per core
#SBATCH --mem-per-cpu=4G            # memory per core
#SBATCH --time=00:10:00             # Wall Time 10mn
#SBATCH --account=formation_isae       # MANDATORY : account  ( launch myaccounts to list your accounts)
#SBATCH --qos=cpu_2019_40
#SBATCH --partition=cpu19_rh8
#SBATCH --export=none               # To start the job with a clean environnement and source of ~/.bashrc

# The following command is mandatory with --export=none option for multinodes jobs
unset SLURM_EXPORT_ENV

# SLURM_SUBMIT_DIR correspondis to directory where sbatch command has been launched
cd ${SLURM_SUBMIT_DIR}

# Run MPI program (same repository)
srun ./sort_single_array_exe
