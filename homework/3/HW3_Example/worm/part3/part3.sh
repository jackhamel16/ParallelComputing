#!/bin/bash --login
########## Define Resources Needed with SBATCH Lines ##########
 
#SBATCH --time=10:00:00             # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --ntasks=1                  # number of tasks - how many tasks (nodes) that you require (same as -n)
#SBATCH --cpus-per-task=1           # number of CPUs (or cores) per task (same as -c)
#SBATCH --mem=40G                    # memory required per node - amount of memory (in bytes)
#SBATCH --job-name Name_of_Job      # you can give your job a name for easier identification (same as -J)
 
 
########## Command Lines to Run ##########
 
module load GCC/6.4.0-2.28
module load OpenMPI/2.1.1
module load BWA/0.7.17                   ### load necessary modules, e.g.

cd /mnt/home/hameljac/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/worm

for i in {1..3}
do
time srun -n 1 bwa mem -t 1 Caenorhabditis_elegans.WBcel235.dna.toplevel.fa.gz DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_1.fq.gz DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_2.fq.gz > DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_1t_aln.sam 
done

js -j $SLURM_JOB_ID -F 
#scontrol show job $SLURM_JOB_ID     ### write job information to output file
