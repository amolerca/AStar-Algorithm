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
    return (x-x0)*(x-x0) + (y-y0)*(y-y0);
}


unsigned long * FindStartAndGoalNodes( Node * node, unsigned long nnodes, double lat_start, double lon_start, double lat_goal, double lon_goal){
    double min_norm_start = 10000000000, min_norm_goal = 10000000000, tmp_norm;
    unsigned long id_start, id_goal;
    double current_lat, current_lon;
    double starting_lat, starting_lon, goal_lat, goal_lon;
    
    for(int i = 0; i< nnodes; i++)
    {
        current_lat = node[i].lat;
        current_lon = node[i].lon;

        if( ( tmp_norm = Euclidean_distance(current_lat, current_lon, lat_start, lon_start) ) < min_norm_start){
            id_start = node[i].id;
            min_norm_start = tmp_norm; 
            starting_lat = node[i].lat;
            starting_lon = node[i].lon;
        }

        if( ( tmp_norm = Euclidean_distance(current_lat, current_lon, lat_goal, lon_goal) ) < min_norm_goal){
            id_goal = node[i].id;
            min_norm_goal = tmp_norm; 
            goal_lat = node[i].lat;
            goal_lon = node[i].lon;
        }
    }

    unsigned long * ids = (unsigned long *) malloc(sizeof(unsigned long) * 2);
    ids[0] = id_start;
    ids[1] = id_goal;

    printf("Node start id %lu, latitude %.5lf, longitude %.5lf \n", ids[0], starting_lat, starting_lon);
    printf("Node goal  id %lu, latitude %.5lf, longitude %.5lf \n", ids[1], goal_lat, goal_lon);

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
    

    printf("Finding node start and node goal...\n");
    //Timing the program
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    
    // Sant Joan de Vilatorrada to UAB
    /*
    double lat_start = 41.76155929999999 , lon_start = 1.797712400000023; //41.38334 |  2.18177
    double lat_goal = 41.5008327, lon_goal = 2.1072626000000128; //37.38610 | -5.99174
    */
    
    /*Barna to Sevilla*/
    double lat_start = 41.38334 , lon_start =2.18177; //41.38334 |  2.18177
    double lat_goal = 37.38610, lon_goal = -5.99174; //37.38610 | -5.99174
      
     
    unsigned long * ids = FindStartAndGoalNodes(node, nnodes, lat_start,  lon_start,  lat_goal,  lon_goal);
    
    // End timing
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("------------------------------------------------------------"
               "\n");
    printf("Completed in %.2f CPU seconds.\n", cpu_time_used);
    printf("------------------------------------------------------------"
               "\n\n");

    // Catalonia
    //AStar(node, nnodes, 771979683, 429854583);

    // Spain
    //AStar(node, nnodes, 240949599, 195977239);
    /*AStar(node, nnodes, args.starting_node, args.ending_node,
          args.heuristic_method, args.weight_method, args.output_file);*/

    AStar(node, nnodes, ids[0], ids[1],
          args.heuristic_method, args.weight_method, args.output_file);
}

