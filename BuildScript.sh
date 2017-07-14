module load /afs/crc.nd.edu/user/n/nsl/nuclear/startup/nsl
module load root
cd /afs/crc.nd.edu/group/nsl/nuc/user/sstrauss/iceball_may2016/Code/Cuts-Parallel
make clean
echo "Rebuilding..."
make
echo "Done rebuild"
echo "Running..."
qsub QueueScript.sh

exit $?
