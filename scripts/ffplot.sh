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
makeDirectory results/charts/ff

# $1 = nw , $2 = throughput
plotffDefault(){
	echo " ... main-default $1 $2 "
	
	makeDirectory results/charts/ff/default/$1_$2
	for (( i=1; i<=3; i++ ))
	do  
		# --ycol 2 refers to the thoughput
		graph results/ff/default/$1_$2/result$i.csv  --marker '' --xcol 1	\
		--ycol 2 --color=#000000 --title "ffmain-default $1 nw , $2 task/milliseconds" --no-grid --xscale 200	\
			-o results/charts/ff/default/$1_$2/result_t$i.png

		# --ycol 3 refers to the number of workers
		graph results/ff/default/$1_$2/result$i.csv  --marker '' --xcol 1	\
			--ycol 3 --color=#000000 --title "ffmain-default $1 nw , $2 task/milliseconds " --no-grid --xscale 200	\
				-o results/charts/ff/default/$1_$2/result_nw$i.png
	done
}


# plotting


echo "plotting ffmain-default..."
makeDirectory results/charts/ff/default

plotffDefault 8 1

plotffDefault 16 2



# $1 = nw , $2 = throughput , $3 = name
plotffOthers(){
	
	makeDirectory results/charts/ff/$3
	for (( i=1; i<=3; i++ ))
	do  
		# --ycol 2 refers to the thoughput
		graph results/ff/$3/result$i.csv  --marker '' --xcol 1	\
		--ycol 2 --color=#000000 --title "ffmain-$3 $1 nw , $2 task/milliseconds" --no-grid --xscale 200	\
			-o results/charts/ff/$3/result_t$i.png

		# --ycol 3 refers to the number of workers
		graph results/ff/$3/result$i.csv  --marker '' --xcol 1	\
			--ycol 3 --color=#000000 --title "ffmain-$3 $1 nw , $2 task/milliseconds " --no-grid --xscale 200	\
				-o results/charts/ff/$3/result_nw$i.png
	done
}


echo "plotting ffmain-constant"
plotffOthers 4 1 constant

