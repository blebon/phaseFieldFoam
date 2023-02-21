#!/bin/bash -l

#SBATCH --account=bddir15  # Run job under project <project>
#SBATCH --time=1:0:0       # Run for a max of 1 hour
#SBATCH --partition=infer  # Choose either "gpu" or "infer" node type
#SBATCH --nodes=1          # Resources from a single node
#SBATCH --gres=gpu:1       # One GPU per node (plus 25% of node CPU and RAM per GPU)
#SBATCH --output=OF10_Phase_%j.out

pwd; hostname; date

module load OpenFOAM/10/Opt

conda activate base

# Source tutorial run functions
source $WM_PROJECT_DIR/bin/tools/RunFunctions

# Get application name
export application=`getApplication`

# Set to number of procs
foamDictionary system/decomposeParDict -entry numberOfSubdomains -set 16
foamDictionary system/decomposeParDict -entry hierarchicalCoeffs/n -set "( 4 4 1 )"
# sed -i 's/(400 400 1)/(4000 4000 1)/g' system/blockMeshDict

./Allrun

date

#------------------------------------------------------------------------------