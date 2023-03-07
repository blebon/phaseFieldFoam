#!/bin/bash -l

#SBATCH --account=bddir15  # Run job under project <project>
#SBATCH --time=2:0:0       # Run for a max of 2 h
#SBATCH --partition=gpu    # Choose either "gpu" or "infer" node type
#SBATCH --nodes=1          # Resources from a single node
#SBATCH --gres=gpu:2       # Two GPUs per node (plus 25% of node CPU and RAM per GPU)
#SBATCH --job-name=OF10_NvcppOpt_Phase
#SBATCH --output=OF10_Phase_%j.out
#SBATCH --mail-user=<your.email>@brunel.ac.uk

pwd; hostname; date

# module load gcc/10.2.0; module load cmake; module load boost; module load vtk;
module load /nobackup/projects/bddir15/hpc_sdk/modulefiles/nvhpc/23.1
export NVLOCALRC=$HOME/localrc
source $HOME/OpenFOAM/${USER}-10/etc/bashrc WM_COMPILER=Nvcpp
unset FOAM_SIGFPE

conda activate base

# Source tutorial run functions
source $WM_PROJECT_DIR/bin/tools/RunFunctions

# Get application name
export application=`getApplication`

# Set to number of procs
# foamDictionary system/decomposeParDict -entry numberOfSubdomains -set 16
# foamDictionary system/decomposeParDict -entry hierarchicalCoeffs/n -set "( 4 4 1 )"
# sed -i 's/(400 400 1)/(4000 4000 1)/g' system/blockMeshDict

nvidia-smi > log.nvidia-smi

if [ ! -f log.decomposePar ]; then
    ./Allrun.pre
fi

nProcs=$(foamDictionary system/decomposeParDict -entry numberOfSubdomains -value)
echo "Running $application in parallel on $(pwd) using $nProcs processes"
# /nobackup/projects/bddir15/hpc_sdk/Linux_ppc64le/23.1/comm_libs/mpi/bin/mpirun -n $nProcs --bind-to none ./bind.sh $FOAM_USER_APPBIN/$application -parallel > log.$application 2>&1
nsys profile -t nvtx,cuda -o report_bind -f true  --stats=true /nobackup/projects/bddir15/hpc_sdk/Linux_ppc64le/23.1/comm_libs/mpi/bin/mpirun -n $nProcs $FOAM_USER_APPBIN/$application -parallel > log.$application # 2>&1
runApplication reconstructPar

./Allpost

date

#------------------------------------------------------------------------------
