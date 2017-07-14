#!/bin/tcsh

#$ -M sstrauss@nd.edu
#$ -m ae
#$ -q debug
#$ -N ICEMerge
#$ -o logs/
#$ -j y

module load /afs/crc.nd.edu/user/n/nsl/nuclear/startup/nsl
module load root
cd /afs/crc.nd.edu/group/nsl/nuc/users/sstrauss/iceball_mar2016/Code/Merge-Parallel
make clean
make
./main

exit $?
