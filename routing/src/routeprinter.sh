#!/bin/bash
# -*- coding: utf-8 -*-

completedir=$(pwd)
parentfolder=$(echo $completedir | sed -e 's/\/.*\///g')

if [ "$parentfolder" == "AStar-Algorithm" ];
    then
    path=$completedir
    path+="/routing/src/routeprinter.py"
    python $path $@
elif [ "$parentfolder" == "routing" ];
    then
    path=$completedir
    path+="/src/routeprinter.py"
    python $path $@
elif [ "$parentfolder" == "src" ];
    then
    path=$completedir
    path+="/routeprinter.py"
    python $path $@
else
    echo "Error: you are not in the repository folder. This script must be executed from the main repository folder"
fi
