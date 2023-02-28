#!/bin/bash -l

# Parallel environment and number of procs
#$ -pe mpi 40

# Request 1 h of wallclock time
#$ -l h_rt=1:0:0

# Working directory
#$ -cwd

# Job name
#$ -N OF10_phase_Nvcpp

# Email alerts
#$ -m be
#$ -M Bruno.Lebon@brunel.ac.uk

#$ -P Test
#$ -A Brunel_allocation


pwd; hostname; date

module purge
module load userscripts
module load beta-modules
module load compilers/nvidia/hpc-sdk/22.9
module load cuda/10.1.243/gnu-4.9.2
module load gcc-libs/4.9.2
module load cmake/3.21.1
module load flex/2.5.39
module load parallel
module load dos2unix
module load bison

# source $HOME/OpenFOAM/OpenFOAM-10/etc/bashrc
source /lustre/home/${USER}/Scratch/OpenFOAM/${USER}-10/etc/bashrc WM_COMPILER=Nvcpp

# Load python3
module load python3

# Source tutorial run functions
source $WM_PROJECT_DIR/bin/tools/RunFunctions

# Get application name
export application=`getApplication`

# Set to number of procs
foamDictionary system/decomposeParDict -entry numberOfSubdomains -set 40
foamDictionary system/decomposeParDict -entry hierarchicalCoeffs/n -set "( 8 5 1 )"
sed -i 's/(400 400 1)/(4000 4000 1)/g' system/blockMeshDict

./Allrun

date

#------------------------------------------------------------------------------
