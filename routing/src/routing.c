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
int main()
{
    Node *node;
    unsigned long nnodes;
    const char BIN_DIR[100] = "bin/map.bin";

    // Read graph
    node = ReadBin(BIN_DIR, &nnodes);
    /*for(int i = 0; i < nnodes; i++){
    	printf("Name current node: %s\n", node[i].name);

    }*/

    printf("\n Distance functions available:\n");
    printf("        1: Haversine\n");
    printf("        2: Spherical law of cosines\n");
    printf("        3: Equirectangular approximation\n");

    Distance_function heuristic; // = &Haversine_distance;
    Distance_function edge_weight;// = &Spherical_law_of_cosines_distance;

    int h_choice, w_choice;

    printf("Choose a heuristic distance (enter the corresponding number): ");
    scanf("%d", &h_choice);

    printf("Choose a wight distance between edges (enter the corresponding number): ");
    scanf("%d", &w_choice);

    if(h_choice == 1){heuristic = &Haversine_distance;}
    else if(h_choice == 2){heuristic = &Spherical_law_of_cosines_distance;}
    else if(h_choice == 3){heuristic = &Equirectangular_distance;}
    else{
        printf("Incorrect distance number identifier, using default distance: Haversine\n");
        heuristic= &Haversine_distance;
    }

    if(w_choice == 1){edge_weight = &Haversine_distance;}
    else if(w_choice == 2){edge_weight = &Spherical_law_of_cosines_distance;}
    else if(w_choice == 3){edge_weight = &Equirectangular_distance;}
    else{
        printf("Incorrect distance number identifier, using default distance: Haversine\n");
        edge_weight = &Haversine_distance;
    } 


    // Catalonia
    //AStar(node, nnodes, 771979683, 429854583);

    // Spain
    
    AStar(node, nnodes, 240949599, 195977239, heuristic, edge_weight);
}
