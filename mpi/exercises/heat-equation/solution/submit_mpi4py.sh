#!/bin/bash
#SBATCH --job-name=job_mpi4py
#SBATCH -N 1 # number of nodes
#SBATCH --ntasks-per-node=4 # MPI processus per node
#SBATCH --ntasks-per-core=1 # MPI processus per node
#SBATCH --mem=180G
#SBATCH --partition=cpu19_rh8
#SBATCH --qos=cpu_2019_80
#SBATCH --time=00:10:00
#SBATCH --account=cnes_level2  # MANDATORY option - launch myaccounts to list  your SLURM accounts
#SBATCH --export=none
#
# with --export=none the following command is mandatory
unset SLURM_EXPORT_ENV

set OMP_NUM_THREADS=1 
# load mpi4py module on TREX RH8
module load mpi4py/3.1.5-py3.8.4-openmpi4.1.2

cd "${SLURM_SUBMIT_DIR}"

# launch // calculation with srun
srun python ./heat-p2p.py  > heat_mpi4py_$SLURM_JOBID.log
