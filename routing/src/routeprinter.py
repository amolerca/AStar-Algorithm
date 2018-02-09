#!/Users/martimunicoy/anaconda2/bin/python
# -*- coding: utf-8 -*-

"""
Small python script to make use of pygmaps to print out the AStar route on a
map from Google Maps.
"""

import argparse
import webbrowser
import os
import gmplot


# Place map in the center of Spain
gmap = gmplot.GoogleMapPlotter(40.4183083, -3.70275, 6)

# Read command-line argument which will be the input file containing the route
# coordinates
parser = argparse.ArgumentParser(description='Print out route on Google Maps')
parser.add_argument('-f', '--file', metavar='FILE',
                    help='directory to a suitable input file')
args = parser.parse_args()
input_file = args.file

# If no command-line argument is specified, use a default path
if input_file is None:
    input_file = 'routes/path1.out'

# Define an output file name according to input file name
output_file = input_file.split('.')[-2] + ".html"

# Read coordinates from input_file
lats = []
lons = []
with open(input_file, 'r') as f:
    for line in f:
        if line.startswith("#"):
            continue
        fields = line.split('|')
        for field in fields:
            field.strip()
        lats.append(float(fields[2]))
        lons.append(float(fields[3]))

# Add route to map
gmap.plot(lats, lons, 'cornflowerblue', edge_width=6)

# Save map
gmap.draw(output_file)
print "File \'{}\' has been created.".format(output_file)

# Show map
f = webbrowser.open("file://" + os.path.realpath(output_file))
