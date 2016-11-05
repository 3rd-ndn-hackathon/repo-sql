#!/bin/bash
make_data_dir="mkdata/"
mkdata="ndnmkdata"
repo_socket="localhost 7376"

# make a set of data and insert to repo-ng
#cd $make_data_dir
make clean
make
#cp $mkdata ../

function randomSuffix()
{
	local nComponents=`echo $((1 + RANDOM % 3))`
	local suff=""
	for c in $(seq 1 $nComponents); do
		comp=`echo $((1 + RANDOM % 10))`
		suff+="/${comp}"
	done;
	echo $suff
}

idx=0
for c1 in {0..10}; do 
	for c2 in {0..999}; do
		suffix=$(randomSuffix)
		./ndnmkdata -D /repo/$c1/$c2$suffix | nc localhost 7376
	done;
	echo "${idx}: inserted 1000 objects."
	let idx=idx+1
done;

# fetch data back from repo-ng
sudo python consumer.py