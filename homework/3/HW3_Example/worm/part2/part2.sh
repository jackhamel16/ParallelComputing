#!/bin/sh

#SBATCH -n 1
#SBATCH --cpus-per-task=1
#SBATCH --mem 40GB
#SBATCH --time 4:00:00

module -q load GCC/6.4.0-2.28
module -q load OpenMPI/2.1.1
module -q load GNU/6.4.0-2.28
module -q load icc/2017.4.196-GCC-6.4.0-2.28
module -q load impi/2017.3.196
module -q load ifort/2017.4.196-GCC-6.4.0-2.28
module -q load BWA/0.7.17

cd /mnt/home/hameljac/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/worm/part2

for i in {1..3}
do
echo "RUN: " $i
echo "TIME FOR PART 1: \n"
time srun -n 1 bwa index -a bwtsw DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_1.fq.gz  
echo "RUN: " $i
echo "TIME FOR PART 2: \n"
time srun -n 1 bwa index -a bwtsw DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_2.fq.gz  
echo "RUN: " $i
echo "TIME FOR TOPLEVEL: \n"
time srun -n 1 bwa index -a bwtsw Caenorhabditis_elegans.WBcel235.dna.toplevel.fa.gz

done

js -j $SLURM_JOB_ID -F

