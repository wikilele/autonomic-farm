#!/bin/bash

#
# This file has been runned on the XEON phi
# Then the result directory should be moved on local machine to plot the charts
#


mkdir results

# running al the experiments and getting the csv files

echo "running main-default"
build/main-default 4 1 > results/result-default41.csv

echo "running main-updown"
build/main-updown 4 1 > results/result-updown41.csv

echo "running main-constant"
build/main-constant 4 1 > results/result-constant41.csv

echo "running main-halfdefault"
build/main-halfdefault 4 2 > results/result-halfdefault42.csv

echo "running main-highlow"
build/main-highlow 4 1 > results/result-highlow41.csv

echo "running main-lowhigh"
build/main-lowhigh 4 1 > results/result-lowhigh41.csv


