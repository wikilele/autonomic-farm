#!/bin/bash


makeDirectory(){
	if [ ! -d $1 ] 
	then
    	mkdir $1
    fi
}

makeDirectory build
makeDirectory build/lib
makeDirectory build/lib/inputvectors


echo "building fast-flow implementation..."
make all-ff



