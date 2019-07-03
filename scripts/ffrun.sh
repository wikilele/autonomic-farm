#!/bin/bash


makeDirectory(){
	if [ ! -d $1 ] 
	then
    	mkdir $1
    fi
}

# $1 = nw , $2 = throughput
runffDefault(){ 
	makeDirectory results/ff/default/$1_$2
	for (( i=1; i<=3; i++ ))
	do  
		echo "...ffmain-default $1 $2 run $i"
   		build/ffmain-default $1 $2 > results/ff/default/$1_$2/result$i.csv
	done
}

makeDirectory results
makeDirectory results/ff

# running al the experiments and getting the csv files

echo "running ffmain-default ..."
makeDirectory results/ff/default

runffDefault 8 1

runffDefault 16 2


# running the others

# $1 = nw , $2 = throughput, $3 = name
runffOthers(){ 
	makeDirectory results/ff/$3
	for (( i=1; i<=3; i++ ))
	do  
		echo "...ffmain-$3 $1 $2 run $i"
   		build/ffmain-$3 $1 $2 > results/ff/$3/result$i.csv
	done
}

echo "running ffmain-constant..."
runffOthers 6 1 constant
