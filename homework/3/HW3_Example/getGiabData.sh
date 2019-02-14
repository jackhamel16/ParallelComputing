#{ time wget --append-output=~/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/logfile ftp://ftp-trace.ncbi.nih.gov/giab/ftp/data/NA12878/Garvan_NA12878_HG001_HiSeq_Exome/NIST7035_TAAGGCGA_L001_R1_001.fastq.gz > /dev/null; } &>> ~/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/download_times.txt

#{ time wget --append-output=~/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/logfile ftp://ftp-trace.ncbi.nih.gov/giab/ftp/data/NA12878/Garvan_NA12878_HG001_HiSeq_Exome/NIST7035_TAAGGCGA_L001_R2_001.fastq.gz > /dev/null; } &>> ~/Documents/CMSE401/ParallelComputing/homework/3/HW3_Example/download_times.txt

{ time wget --append-output=logfile ftp://ftp-trace.ncbi.nih.gov/giab/ftp/data/NA12878/Garvan_NA12878_HG001_HiSeq_Exome/NIST7035_TAAGGCGA_L001_R1_001.fastq.gz > /dev/null; } &>> download_times.txt

{ time wget --append-output=logfile ftp://ftp-trace.ncbi.nih.gov/giab/ftp/data/NA12878/Garvan_NA12878_HG001_HiSeq_Exome/NIST7035_TAAGGCGA_L001_R2_001.fastq.gz > /dev/null; } &>> download_times.txt
