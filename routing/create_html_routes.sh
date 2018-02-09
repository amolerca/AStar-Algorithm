#!/bin/bash

for file in routes/path*.out 
do   
	./routeprinter.exe -f $file;
done;
