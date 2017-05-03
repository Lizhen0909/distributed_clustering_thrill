#!/bin/bash
#MSUB -N preprocess
#MSUB -l nodes=4:ppn=16
#MSUB -l walltime=3:00:00
#MSUB -l mem=256000mb
#MSUB -q multinode
#MSUB -v MPI_MODULE=mpi/openmpi/2.0-gnu-5.2
#MSUB -v THRILL_WORKERS_PER_HOST=16
#MSUB -v MPIRUN_OPTIONS="--bind-to core --map-by node:PE=16 -report-bindings"

module load ${MPI_MODULE}

echo "${MOAB_JOBNAME} running on ${MOAB_PROCCOUNT} cores with ${MOAB_NODECOUNT} MPI-tasks and ${THRILL_WORKERS_PER_HOST} threads"

executable="$HOME/code/prototypes/thrill_louvain/release/preprocess"
startexe="mpirun -n ${MOAB_NODECOUNT} ${MPIRUN_OPTIONS} ${executable}"
echo $startexe "$GRAPH" "$GROUNDTRUTH"
exec $startexe "$GRAPH" "$GROUNDTRUTH"
rm "$GRAPH" "$GROUNDTRUTH"
