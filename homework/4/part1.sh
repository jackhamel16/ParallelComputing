#!/bin/sh

#SBATCH -N 1
#SBATCH -n 1
#SBATCH --mem 1GB
#SBATCH --time 00:10:00

module load libpng

make

if [ `ls | grep out_images -c` -eq 1 ]
then
  echo "here"
  if [ `ls out_images/ | wc -l` -ne 0 ]
  then
    rm -r out_images/*
  fi
else
  echo "there"
  mkdir out_images
fi

> part1_timings.dat

for img in cube.png  earth.png  MSUStadium.png  sparty.png
do
  for i in 1 2 3
  do
  echo $img "Run: " $i >> part1_timings.dat
  { time ./process images/$img out_images/{$img}.o; } &>> part1_timings.dat
  done
done
