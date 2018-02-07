#!/bin/bash
# -*- coding: utf-8 -*-

completedir=$(pwd)
parentfolder=$(echo $completedir | sed -e 's/\/.*\///g')

if [ "$parentfolder" == "AStar-Algorithm" ];
    then
    path=$completedir
    path+="/routing/src/routeprinter.py"
elif [ "$parentfolder" == "routing" ];
    then
    path=$completedir
    path+="/src/routeprinter.py"
elif [ "$parentfolder" == "src" ];
    then
    path=$completedir
    path+="/routeprinter.py"
else
    echo "Error: not in the repository folder. This script must be executed from the main repository folder"
fi

python $path $@
