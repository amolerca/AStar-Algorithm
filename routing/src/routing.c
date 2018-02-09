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

    // Start timing
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Sant Joan de Vilatorrada to UAB
    /*
    double lat_start = 41.76155929999999 , lon_start = 1.797712400000023; //41.76155929999999,1.797712400000023
    double lat_goal = 41.5008327, lon_goal = 2.1072626000000128; //41.5008327,2.1072626000000128
    */
    /*Barna to Sevilla*/

    //double lat_start = 41.38334 , lon_start =2.18177; //41.38334,2.18177
    //double lat_goal = 37.38610, lon_goal = -5.99174; //37.38610,-5.99174

    printf("------------------------------------------------------------\n");
    printf("Locating starting and ending nodes in the graph...\n");
    printf("------------------------------------------------------------\n");

    unsigned long starting_id, ending_id;
    ParseInputPoint(&starting_id, args.starting_point, node, nnodes, "start");
    ParseInputPoint(&ending_id, args.ending_point, node, nnodes, "end");

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

    AStar(node, nnodes, starting_id, ending_id, args.heuristic_method,
          args.weight_method, args.output_file);
}

