# AStar assignment - A routing problem

## Introduction
This is an assignment from the Optimisation subject at Master's in modelling for Science and Engineering in the UAB.
Professor Lluís Alsedà (http://mat.uab.cat/~alseda/MasterOpt/index.html).

The assignment consists in computing an optimal path (according to distance) from *Basílica de Santa
Maria del Mar* (Plaça de Santa Maria) in Barcelona to the *Giralda* (Calle Mateos Gago) in Sevilla by
using the `AStar` algorithm. To this end one has to implement the `AStar` algorithm (compulsory in form
of a function - not inside the main code) and compute and write the optimal path.

As the reference starting node for *Basílica de Santa Maria del Mar* (Plaça de Santa Maria) in
Barcelona we will take the node with key `(@id): 240949599` while the goal node close to *Giralda* (Calle
Mateos Gago) in Sevilla will be the node with key `(@id): 195977239`.

## Authors
* **Martí Municoy**, mail@martimunicoy.com
* **Dani Salgado**, daniel.salgado@e-campus.uab.cat

## Description

We introduce a program which makes use of the A* algorithm to solve general routing problems.
We have added the possibility of choosing the locations by entering the latitude and longitude coordinates of the starting and goal locations.
Then, the program finds the nearest existing nodes to these pairs of coordinates and computes the optimal path between them, if there exists any.
Our A* implementation in C language has been proved to be a reliable tool to solve a general routing problem to find paths that minimize distance.
We assume that the data sets for the regions of interest are available and are formatted in a proper way.
In addition, we have added a script in Python that is able to represent the paths graphically as it is done by Google Maps.


A detailed explanation of the installation and the execution of the program is found below.

## Getting started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
Depending on the native OS some external libraries will need to be included to run this program. See install section below.

This project supports the following C compiler versions:
* Apple gcc v4.2.1
* Ubuntu (16.4) gcc v5.4.0

Other regular C compilers should work as well but have not been tested yet.

For a graphical representation of the paths via Google Maps, Python 2.7 needs to be installed and the following (additional) packages are required:
   * _matplotlib_
   * _requests_

### Installing with Make

Setting a running copy of this project with `Make` is straightforward.
Make can build the project executables according to the user preferences.
From the main directory of the project you can run several `Make`commands with different predefined built in behaviors.
They are summarized below:

   * Type `make` to install all executables
   * Type `make install` to install only the main executables of the program
   * Type `make test` to only install the testing executable of the program
   * Type `make clean` to remove all executables

In principle, testing executables do not offer useful or reliable data.
They do not offer a good user experience and their use is recommended only for development purposes.

Besides the installation with `Make`, this program includes a Python script which can represent graphically yhe paths on a Google Maps map.
To run this script a proper Python environment needs to be configured.
There different ways to get this Python script working. We recommend you two different methods depending on your system architecture.

### Getting Python libraries on OSX

The easiest way to set the required packages up is by installing [Conda](https://conda.io/docs/).
Download the suitable binary file from their oficial [website](https://conda.io/docs/user-guide/install/macos.html) and follow the instructions from there.

With a working `Python 2.7` Conda environment set up, you can run the following commands to install the required packages from above:

```
conda install matplotlib
```

```
conda install requests
```

### Installing on Linux

Additionally, you can get a proper Python environment by using the [Python Package Index](https://pip.pypa.io/en/stable/) tool (`pip`).
It can be installed in a Debian/Ubuntu based system by using the following command:
```
sudo apt install python-pip
```

Then, we just need to ask pip to install the required Python packages:

```
pip install matplotlib
```

```
pip install requests
```


## Instructions
The default `Make` installation is the following one:
```
make install
```
It will compile 4 executables which are listed and briefly explained below:

   * **bincreator.exe**: it parses a suitable input map file (_.map_) and saves the corresponding graph to a binary file (_.bin_).
   * **cmapcreator.exe**: it parses a suitable input map file (_.map_) and saves the corresponding graph to a compressed binary file (_.cmap_).
   * **routing.exe**: it takes the graph previously parsed by either **bincreator.exe** (_.bin_) or **cmapcreator.exe** (_.cmap_) and uses it to find the shortest route between the two input points.
   * **routeprinter.exe**: it plots the route obtained by **routing.exe** (_.out_) on a map from Google Maps (it requires a proper Python 2.7 environment).

## Test running

To be continued.

## References

   * [1] A\*-Algorithm GitHub repository, Martí Municoy and Daniel Salgado, https://github.com/dsalgador/AStar-Algorithm

   * [2] A\* search algorithm. (2018, January 28). In Wikipedia, The Free Encyclopedia. Retrieved 18:34, February 7, 2018, from https://en.wikipedia.org/w/index.php?title=A*_search_algorithm\&oldid=822855961

   * [3] Admissible heuristic. (2017, December 7). In Wikipedia, The Free Encyclopedia. Retrieved 19:25, February 7, 2018, from https://en.wikipedia.org/w/index.php?title=Admissible_heuristic\&oldid=814117910

   * [4] Heuristics: Intelligent Search Strategies for Computer Problem Solving, Judea Pearl. Addison-Wesley Pub (Sd), ISBN: 0201055945, 1984-04

   * [5] Calculate distance, bearing and more between Latitude/Longitude points, Chris Veness. http://www.movable-type.co.uk/scripts/latlong.html

   * [6] Haversine formula. (2017, December 31). In Wikipedia, The Free Encyclopedia. Retrieved 00:31, February 8, 2018, from https://en.wikipedia.org/w/index.php?title=Haversine_formula\&oldid=817871096

   * [7] Earth radius. (2018, January 22). In Wikipedia, The Free Encyclopedia. Retrieved 15:45, February 7, 2018, from https://en.wikipedia.org/w/index.php?title=Earth_radius\&oldid=821830040

   * [8] P. E. Hart, N. J. Nilsson and B. Raphael, A Formal Basis for the Heuristic Determination of Minimum Cost Paths, in IEEE Transactions on Systems Science and Cybernetics, vol. 4, no. 2, pp. 100-107, July 1968. doi: 10.1109/TSSC.1968.300136. http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=\&arnumber=4082128\&isnumber=4082123

   * [9] Latitude, longitude and address of any point on Google Maps, https://www.gps-coordinates.net

## License
This project is licensed under the GNU License - see the [LICENSE](LICENSE) file for details

