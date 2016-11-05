#!/bin/bash
make_data_dir="mkdata/"
mkdata="ndnmkdata"
repo_socket="localhost 7376"

# make a set of data and insert to repo-ng
#cd $make_data_dir
make clean
make
#cp $mkdata ../

repoCmd="nc localhost 7376"

if [ $# -lt 1 ]; 
    then echo "illegal number of parameters"
    exit 1
fi

if [ $1 == "sql" ]; then
	echo "using SQL repo..."
	repoCmd="repo-sql-insert"
else
	echo "using repo-ng..."
fi

# if [ $2 == "clean" ]; then 
# 	echo "cleaning repo"
# 	if [ $1 == "sql" ]; then
# 		echo ""
# 	else
# 		sudo serivce repo-ng stop
# 		sudo rm /var/lib/ndn/repo-ng/*
# 		sudo service repo-ng start
# 	fi
# 	exit 0
# fi

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

START=$(date +%s.%N)

idx=0
for c1 in {0..10}; do 
	for c2 in {0..999}; do
		suffix=$(randomSuffix)
		if [ $1 == "sql" ]; then
			./ndnmkdata -D /repo/$c1/$c2$suffix | repo-sql-insert
		else
			./ndnmkdata -D /repo/$c1/$c2$suffix | nc localhost 7376
		fi
	done;
	echo "${idx}: inserted 1000 objects."
	let idx=idx+1
done;

END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)

echo "insertion took $DIFF seconds"

# fetch data back from repo-ng
# sudo python consumer.py