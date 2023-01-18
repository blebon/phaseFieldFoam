#!/bin/bash -l

# Parallel environment and number of procs
#$ -pe mpi 40

# Request 1 h of wallclock time
#$ -l h_rt=1:0:0

# Working directory
#$ -cwd

# Job name
#$ -N OF10_phase

# Email alerts
#$ -m be
#$ -M Bruno.Lebon@brunel.ac.uk

#$ -P Test
#$ -A Brunel_allocation


pwd; hostname; date

module purge
module load userscripts
module load beta-modules
module load gcc-libs/10.2.0
module load compilers/gnu/10.2.0
module load mpi/openmpi/4.0.5/gnu-10.2.0
module load cmake/3.21.1
module load flex/2.5.39
module load parallel
module load dos2unix
module load bison

# source $HOME/OpenFOAM/OpenFOAM-9/etc/bashrc
source /lustre/home/mmm0708/OpenFOAM/OpenFOAM-10/etc/bashrc

# Load python3
module load python3

# Source tutorial run functions
source $WM_PROJECT_DIR/bin/tools/RunFunctions

# Get application name
export application=`getApplication`
./Allrun

date

#------------------------------------------------------------------------------
