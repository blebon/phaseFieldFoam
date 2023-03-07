#!/bin/bash
# From Filippo Spiga (NVIDIA)

case ${SLURM_LOCALID} in
    0)
        export CUDA_VISIBLE_DEVICES=0
        CPU_BIND=0-63
        ;;
    1)
        export CUDA_VISIBLE_DEVICES=1
        CPU_BIND=0-63
        ;;
    2)
        export CUDA_VISIBLE_DEVICES=2
        CPU_BIND=64-127
        ;;
    3)
        export CUDA_VISIBLE_DEVICES=3
        CPU_BIND=64-127
        ;;
esac

if [[ ${SLURM_LOCALID} == 0 ]]; then
    nsys profile --trace=cuda,nvtx -o report_bind --stats=true --cuda-memory-usage=true numactl --physcpubind=${CPU_BIND} $* 
else
    numactl --physcpubind=${CPU_BIND} $*
fi
