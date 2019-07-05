#!/bin/bash

#
# This file has been runned on the XEON phi
# Then the result directory should be moved on local machine to plot the charts
#

makeDirectory(){
	if [ ! -d $1 ] 
	then
    	mkdir $1
    fi
}

# $1 = nw , $2 = throughput
runDefault(){ 
	makeDirectory results/default/$1_$2
	for (( i=1; i<=3; i++ ))
	do  
		echo "...main-default $1 $2 run $i"
   		build/main-default $1 $2 > results/default/$1_$2/result$i.csv
	done
}

makeDirectory results


# running al the experiments and getting the csv files

echo "running main-default ..."
makeDirectory results/default

runDefault 4 1

runDefault 1 1

runDefault 8 1

runDefault 4 2

runDefault 4 4

runDefault 16 4


# running the others

# $1 = nw , $2 = throughput, $3 = name
runOthers(){ 
	makeDirectory results/$3
	for (( i=1; i<=3; i++ ))
	do  
		echo "...main-$3 $1 $2 run $i"
   		build/main-$3 $1 $2 > results/$3/result$i.csv
	done
}

echo "running main-updown..."
runOthers 4 1 updown

echo "running main-constant..."
runOthers 4 1 constant

echo "running main-halfdefault..."
runOthers 4 2 halfdefault

echo "running main-highlow..."
runOthers 4 1 highlow

echo "running main-lowhigh..."
runOthers 4 1 lowhigh
