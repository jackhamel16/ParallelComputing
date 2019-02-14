#!/bin/bash --login
########## Define Resources Needed with SBATCH Lines ##########
 
#SBATCH --time=6:00:00             # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --ntasks=16                  # number of tasks - how many tasks (nodes) that you require (same as -n)
#SBATCH --cpus-per-task=16           # number of CPUs (or cores) per task (same as -c)
#SBATCH --mem=40G                    # memory required per node - amount of memory (in bytes)
#SBATCH --job-name Name_of_Job      # you can give your job a name for easier identification (same as -J)
 
 
########## Command Lines to Run ##########
 
module -q load GCC/6.4.0-2.28
module -q load OpenMPI/2.1.1
module -q load BWA/0.7.17                   ### load necessary modules, e.g.

cd /mnt/home/hameljac/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/worm/part4

for num_threads in 2 4 8 16
do
  for i in {1..3}
  do
    echo "THREADS: " $num_threads "RUN: " $i
    time srun -n $num_threads bwa mem -t $num_threads Caenorhabditis_elegans.WBcel235.dna.toplevel.fa.gz DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_1.fq.gz DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_2.fq.gz > DataSet01_Celegans_Paired200Id200Pexp100Cov10E1N0GenomicControl_1t_aln.sam  
  done
done

js -j $SLURM_JOB_ID -F 
#scontrol show job $SLURM_JOB_ID     ### write job information to output file
