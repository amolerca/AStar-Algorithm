#!/bin/bash

reference_file="path11.out"
for file in path*.out 
do 
cmp --silent $file $reference_file || echo "Files $file and $reference_file correspond to different paths."
done;
