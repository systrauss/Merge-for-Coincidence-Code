module load /afs/crc.nd.edu/user/n/nsl/nuclear/startup/nsl
module load root
cd /afs/crc.nd.edu/group/nsl/nuc/users/sstrauss/iceball_may2016/Code/Merge-Parallel
make clean
echo "Rebuilding..."
make
echo "Done rebuild"
echo "Running..."
./main
make clean

exit $?
