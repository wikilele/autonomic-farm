#!/bin/bash

#
# To run this file is needed the graph-cli utility
#	pip3 install graph-cli
#	https://github.com/mcastorina/graph-cli
# 	
# This file has been runned on the local machine 
#	

#rsync -avzh frioli-spm19@unipipds:/home/frioli-spm19/autonomic-farm/results /home/leonardo/Documenti/unipi/ParallelDistributedSystems/finalproject/


mkdir results/charts
OUTPUT_DIR=results/charts

# plotting

echo "plotting main-default"
# --ycol 2 refers to the thoughput
graph results/result-default41.csv  --marker '' --xcol 1	\
	--ycol 2 --color=#000000 --title 'main-default 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-default41t.png

# --ycol 3 refers to the number of workers
graph results/result-default41.csv  --marker '' --xcol 1	\
	--ycol 3 --color=#000000 --title 'main-default 4 1 ' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-default41nw.png


echo "plotting main-updown"
graph results/result-updown41.csv  --marker '' --xcol 1		\
	--ycol 2 --color=#000000 --title 'main-updown 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-updown41t.png

graph results/result-updown41.csv  --marker '' --xcol 1		\
	--ycol 3 --color=#000000 --title 'main-updown 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-updown41nw.png


echo "plotting main-constant"
graph results/result-constant41.csv  --marker '' --xcol 1	\
	--ycol 2 --color=#000000 --title 'main-constant 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-constant41t.png

graph results/result-constant41.csv  --marker '' --xcol 1	\
	--ycol 3 --color=#000000 --title 'main-constant 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-constant41nw.png


echo "plotting main-default"
graph results/result-halfdefault42.csv  --marker '' --xcol 1	\
	--ycol 2 --color=#000000 --title 'main-halfdefault 4 2' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-halfdefault42t.png

graph results/result-halfdefault42.csv  --marker '' --xcol 1	\
	--ycol 3 --color=#000000 --title 'main-halfdefault 4 2' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-halfdefault42nw.png


echo "plotting main-highlow"
graph results/result-highlow41.csv  --marker '' --xcol 1	\
	--ycol 2 --color=#000000 --title 'main-highlow 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-highlow41t.png

graph results/result-highlow41.csv  --marker '' --xcol 1	\
	--ycol 3 --color=#000000 --title 'main-highlow 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-highlow41nw.png


echo "plotting main-lowhigh"
graph results/result-lowhigh41.csv  --marker '' --xcol 1	\
	--ycol 2 --color=#000000 --title 'main-lowhigh 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-lowhigh41t.png

graph results/result-lowhigh41.csv  --marker '' --xcol 1	\
	--ycol 3 --color=#000000 --title 'main-lowhigh 4 1' --no-grid --xscale 200	\
		-o $OUTPUT_DIR/result-lowhigh41nw.png
