#!/bin/bash -l

#SBATCH --export=NONE

#SBATCH --account=bddir15  # Run job under project <project>
#SBATCH --time=2:0:0       # Run for a max of 2 hours

#SBATCH --partition=infer  # Choose either "gpu" or "infer" node type
#SBATCH --nodes=1          # Resources from a single node
#SBATCH --gres=gpu:1       # One GPU per node (plus 25% of node CPU and RAM per GPU)

#SBATCH --job-name=OF10_NvccOpt_myOpenFOAM_wmake_libso
#SBATCH --output=NvccOpt_myOpenFOAM_wmake_libso_%j.out

pwd; hostname; date

module load gcc/10.2.0
module load cmake
module load boost
module load openmpi
module load vtk
module load cuda
module load nvhpc

source ${HOME}/OpenFOAM/${USER}-10/etc/bashrc WM_COMPILE_OPTION=Opt WM_COMPILER=Nvcc

# Source tutorial run functions
source $WM_PROJECT_DIR/bin/tools/RunFunctions

export WM_NCOMPPROCS=160
wmake libso

date

#------------------------------------------------------------------------------

