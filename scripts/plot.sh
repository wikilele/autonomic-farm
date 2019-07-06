#!/bin/bash

#
# To run this file is needed the graph-cli utility
#	pip3 install graph-cli
#	https://github.com/mcastorina/graph-cli
# 	
# This file has been runned on the local machine 
#	



makeDirectory(){
	if [ ! -d $1 ] 
	then
    	mkdir $1
    fi
}

makeDirectory results/charts

# $1 = nw , $2 = throughput
plotDefault(){
	echo " ... main-default $1 $2 "
	
	makeDirectory results/charts/default/$1_$2
	for (( i=1; i<=3; i++ ))
	do  
		# --ycol 2 refers to the thoughput
		graph results/default/$1_$2/result$i.csv  --marker '' --xcol 1	\
		--ycol 2 --color=#000000 --title "main-default $1 nw , $2 task/milliseconds" --no-grid --xscale 300	\
			-o results/charts/default/$1_$2/result_t$i.png

		# --ycol 3 refers to the number of workers
		graph results/default/$1_$2/result$i.csv  --marker '' --xcol 1	\
			--ycol 3 --color=#000000 --title "main-default $1 nw , $2 task/milliseconds " --no-grid --xscale 300	\
				-o results/charts/default/$1_$2/result_nw$i.png
	done
}


# plotting


echo "plotting main-default..."
makeDirectory results/charts/default

plotDefault 4 1

plotDefault 1 1

plotDefault 8 1

plotDefault 4 2

plotDefault 4 4

plotDefault 16 4

# $1 = nw , $2 = throughput , $3 = name
plotOthers(){
	
	makeDirectory results/charts/$3
	for (( i=1; i<=3; i++ ))
	do  
		# --ycol 2 refers to the thoughput
		graph results/$3/result$i.csv  --marker '' --xcol 1	\
		--ycol 2 --color=#000000 --title "main-$3 $1 nw , $2 task/milliseconds" --no-grid --xscale 200	\
			-o results/charts/$3/result_t$i.png

		# --ycol 3 refers to the number of workers
		graph results/$3/result$i.csv  --marker '' --xcol 1	\
			--ycol 3 --color=#000000 --title "main-$3 $1 nw , $2 task/milliseconds " --no-grid --xscale 200	\
				-o results/charts/$3/result_nw$i.png
	done
}

echo "plotting main-updown"
plotOthers 4 1 updown


echo "plotting main-constant"
plotOthers 4 1 constant

echo "plotting main-halfdefault"
plotOthers 4 2 halfdefault

echo "plotting main-highlow"
plotOthers 4 1 highlow

echo "plotting main-lowhigh"
plotOthers 4 1 lowhigh

