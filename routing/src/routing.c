/*****************************************************************************
 *                                                                           *
 *                        <Routing - AStar Algorithm>                        *
 *             Copyright (C) <2018>   <Municoy, M., Salgado, D.>             *
 *                                                                           *
 *   Contact the authors at: mail@martimunicoy.com                           *
 *                           daniel.salgado@e-campus.uab.cat                 *
 *                                                                           *
 *   This program is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *****************************************************************************/

// Include libraries
#include "routing.h"
#include "constants.h"
#include "util.h"
#include "graph.h"
#include "reader.h"
#include "binary.h"

double Euclidean_distance(double x, double y, double x0, double y0){
    return sqrtf( (x-x0)*(x-x0) + (y-y0)*(y-y0) );
}


int * FindStartAndGoalNodes( Node * node, unsigned long nnodes, double lat_start, double lon_start, double lat_goal, double lon_goal){
    double min_norm_start = 1000000, min_norm_goal = 1000000, tmp_norm;
    unsigned long id_start, id_goal;
    double current_lat, current_lon;
    
    for(int i = 0; i< nnodes; i++)
    {
        current_lat = node[i].lat;
        current_lon = node[i].lat;

        if( ( tmp_norm = Euclidean_distance(current_lat, current_lon, lat_start, lon_start) ) < min_norm_start){
            id_start = node[i].id;
            min_norm_start = tmp_norm; 
        }

        if( ( tmp_norm = Euclidean_distance(current_lat, current_lon, lat_goal, lon_goal) ) < min_norm_goal){
            id_goal = node[i].id;
            min_norm_goal = tmp_norm; 
        }

    }

    int * ids = (int *) malloc(sizeof(int) * 2);
    ids[0] = id_start;
    ids[1] = id_goal;

    return ids; 
}



// Main function
int main(int argc, char **argv)
{
    RoutingArguments args;
    SetDefaultRoutingArgs(&args);
    ParseRoutingArgs(argc, argv, &args);
    CheckRoutingArgs(&args);

    Node *node;
    unsigned long nnodes;

    // Read graph
    node = ReadBin(args.input_file, &nnodes);
    
    // Catalonia
    //AStar(node, nnodes, 771979683, 429854583);

    // Spain
    //AStar(node, nnodes, 240949599, 195977239);
    AStar(node, nnodes, args.starting_node, args.ending_node,
          args.heuristic_method, args.weight_method, args.output_file);
}

