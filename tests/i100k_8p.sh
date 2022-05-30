#!/bin/bash
#SBATCH -J i100k_8p
#SBATCH -o i100k_8p_%j.out
#SBATCH -e i100k_8p_%j.err
#SBATCH --mail-user luiggigianpiere.tenorioku@studenti.unipd.it
#SBATCH --mail-type ALL
#SBATCH -n 8
#SBATCH -p allgroups
#SBATCH -t 00:10:00
#SBATCH --mem 1G
cd $SLURM_SUBMIT_DIR
spack load intel-parallel-studio@professional.2019.4
mpirun -np $SLURM_NTASKS ../main ../inputs/input_100000.txt