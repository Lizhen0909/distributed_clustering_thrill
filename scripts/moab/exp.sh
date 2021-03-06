#!/bin/bash
#MSUB -N thrill_exp_test
#MSUB -l nodes=4:ppn=5
#MSUB -l walltime=01:00:00
#MSUB -l mem=256gb
#MSUB -q multinode
#MSUB -v MPI_MODULE=mpi/openmpi/2.0-gnu-5.2
#MSUB -v THRILL_WORKERS_PER_HOST=5
#MSUB -v MPIRUN_OPTIONS="--bind-to core --map-by node:PE=5 -report-bindings"

cat /proc/cpuinfo > cpu-$MOAB_JOBID

export THRILL_LOG="log-${MOAB_JOBID}"
module load ${MPI_MODULE}

echo "${MOAB_JOBNAME} running on ${MOAB_PROCCOUNT} cores with ${MOAB_NODECOUNT} MPI-tasks and ${THRILL_WORKERS_PER_HOST} threads"

executable="$HOME/code/prototypes/thrill_louvain/release/exp"
startexe="mpirun -n ${MOAB_NODECOUNT} ${MPIRUN_OPTIONS} ${executable}"
echo $startexe "$GRAPH"
exec $startexe "$GRAPH"
