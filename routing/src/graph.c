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

// Include Libraries
#include "routing.h"
#include "constants.h"
#include "util.h"
#include "graph.h"

void PrintNodeByIndex(unsigned long index, Node *node, unsigned long nnodes)
{
    if (index >= nnodes)
        ExitError("when printing node. Node index out of range", 66);

    printf("  Node id:            %lu\n", node[index].id);
    printf("  Node name:          %s\n", node[index].name);
    printf("  Node lat:           %f\n", node[index].lat);
    printf("  Node lon:           %f\n", node[index].lon);
    printf("  Node nsucc:         %hu\n", node[index].nsucc);
    printf("  Node asucc:         %hu\n", node[index].nsucc);
    printf("  Node successor ids: ");
    unsigned long i;
    for (i = 0; i < node[index].nsucc; i++)
        printf("%lu ", node[index].successor[i]->id);
    printf("\n");
}

void PrintNodeById(unsigned long id, Node *node, unsigned long nnodes)
{
    if (IsIndexed(node, nnodes))
    {
        if (id >= nnodes)
            ExitError("when printing node. Node id out of range", 66);
    }
    else
    {
        long search_results = BinarySearch(id, node, 0, nnodes - 1);
        if (search_results == -1)
            ExitError("when printing node. Node id not found", 67);
        id = (unsigned long) search_results;
    }

    printf("  Node id:            %lu\n", node[id].id);
    printf("  Node name:          %s\n", node[id].name);
    printf("  Node lat:           %f\n", node[id].lat);
    printf("  Node lon:           %f\n", node[id].lon);
    printf("  Node nsucc:         %hu\n", node[id].nsucc);
    printf("  Node asucc:         %hu\n", node[id].nsucc);
    printf("  Node successor ids: ");
    unsigned long i;
    for (i = 0; i < node[id].nsucc; i++)
        printf("%lu ", node[id].successor[i]->id);
    printf("\n");
}

bool IsIndexed(Node *node, unsigned long nnodes)
{
    return (node[nnodes - 1].id == nnodes - 1);
}

long BinarySearch(unsigned long id, Node *node, unsigned long left,
                           unsigned long right)
{
    if (right >= left)
    {
        unsigned long middle = left + (right - left) / 2;

        // Check for a solution
        if (node[middle].id == id)
            return middle;

        // Node can only be present in the left subarray
        if (node[middle].id > id)
            return BinarySearch(id, node, left, middle - 1);

        // Node can only be present in the right subarray
        return BinarySearch(id, node, middle + 1, right);
   }

   // No node found
   return -1;
}

unsigned long BinarySearchChkd(unsigned long id, Node *node,
                               unsigned long left, unsigned long right,
                               int errnum)
{
    long search_results = BinarySearch(id, node, left, right);

    if (search_results == -1)
        ExitError("when finding node by binary search in graph", errnum);

    return (unsigned long) search_results;
}

void AddSuccessor(Node *A, Node *B)
{
    //printf("%lu -> %lu\n", A->id, B->id);

    if (A->asucc == 0)
    {
        A->asucc = 2;
        A->successor = (Node **) malloc(sizeof(Node *) * A->asucc);
    }

    else if (A->asucc == A->nsucc)
    {
        A->asucc += 2;
        A->successor = (Node **) realloc(A->successor,
                                          sizeof(Node *) * A->asucc);
    }

    A->successor[A->nsucc] = B;
    A->nsucc++;
}

Node *FromIdToNode(unsigned long id, Node *node, unsigned long nnodes)
{
    long search_results;
    unsigned long index;
    search_results = BinarySearch(id, node, 0, nnodes - 1);
    if (search_results == -1){
        return NULL;
    }

    index = (unsigned long) search_results;
    return &node[index];
}

bool CheckNodes(Node *node, unsigned long nnodes)
{
    unsigned long i;
    for (i = 0; i < nnodes - 1; i++)
        if (node[i].id >= node[i + 1].id)
            return false;
    return true;
}

char *GetLinkedNodes(Node *node, unsigned long nnodes)
{
    unsigned long i, j, id;
    char *linked_nodes = (char *) malloc(sizeof(char) * nnodes);
    memset(linked_nodes, false, sizeof(char) * nnodes);

    for (i = 0; i < nnodes; i++)
    {
        if (node[i].nsucc > 0)
        {
            linked_nodes[i] = true;
            for (j = 0; j < node[i].nsucc; j++)
            {
                id = node[i].successor[j]->id;
                linked_nodes[id] = true;
            }
        }
    }

    return linked_nodes;
}

Node *CleanGraph(Node *node, char *linked_nodes, unsigned long nnodes,
                 unsigned long n_del)
{
    unsigned long i, j, id, clean_id;
    unsigned long index = 0;
    long search_results;

    Node *clean_node = (Node *) malloc(sizeof(Node) * nnodes);
    if (clean_node == NULL)
        ExitError("when allocating memory to save the enhanced graph", 6);

    // Add nodes (only the linked ones)
    for (i = 0; i < nnodes + n_del; i++)
        if (linked_nodes[i] == 1)
        {
            //memcpy(&clean_node[i], &node[index], sizeof(Node));
            clean_node[index] = node[i];
            index++;
        }

    // Add successors to new nodes
    index = 0;
    for (i = 0; i < nnodes + n_del; i++)
    {
        if (linked_nodes[i] == 1)
        {
            // Allocate memory for clean nodes new successors
            if (node[i].nsucc)
                clean_node[index].successor = (Node **) malloc(sizeof(Node *) *
                                                           node[i].nsucc);

            // Assign new clean node ids to old node ones
            for (j = 0; j < node[i].nsucc; j++)
            {
                id = node[i].successor[j]->id;
                clean_id = BinarySearchChkd(id, clean_node, 0, nnodes - 1,
                                            140);
                //memcpy(&node[index], &clean_node[i], sizeof(Node));
                clean_node[index].successor[j] = &clean_node[clean_id];
            }

            // Free old successors memory
            if (node[i].nsucc)
                free(node[i].successor);

            // Update clean nodes index
            index++;
        }
    }

    // Free old nodes memory
    free(node);

    // Reindex new nodes
    printf(" Reindexing nodes...\n");
    for (i = 0; i < nnodes; i++)
        clean_node[i].id = i;

    // Return new nodes
    return clean_node;
}

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges)
{
    // Let user know waht we are doing
    printf("Minimizing graph inconsistencies...\n");

    int i;
    unsigned long ln_size;

    // Ids reindexing
    printf(" Reindexing nodes...\n");
    for (i = 0; i < *nnodes; i++)
        node[i].id = i;

    // Get linked nodes
    char *linked_nodes = GetLinkedNodes(node, *nnodes);

    // Get the number of unlinked nodes
    unsigned long n_del = 0;
    for (i = 0; i < *nnodes; i++)
        if (linked_nodes[i] == 0)
            n_del++;

    // Remove unlinked nodes, if any
    if (n_del > 0)
    {
        printf(" Total number of unlinked nodes found: %lu\n", n_del);

        // Substract the number of nodes that will be removed
        *nnodes -= n_del;

        // Copying a clean graph to enh_node
        printf(" Removing unlinked nodes...\n");
        Node *enh_node = CleanGraph(node, linked_nodes, *nnodes, n_del);

        // Display new graph size
        printf("New graph size: \n");
        printf(" Number of nodes: %lu\n", *nnodes);
        printf(" Number of ways: %lu\n", nways);
        printf(" Number of edges: %lu\n", nedges);

        return enh_node;
    }

    printf(" No unlinked nodes were found\n");

    return node;
}

bool AnyOpen(AStarNode *asnode, unsigned long nnodes)
{
    int i;
    for (i = 0; i < nnodes; i++)
        if (asnode[i].stat == OPEN)
            return true;
    return false;
}

double HeuristicHaversine(AStarNode node1, AStarNode node2)
{
    double lat1 = ToRadians(node1.node->lat);
    double lat2 = ToRadians(node2.node->lat);
    double deltalat = lat2 - lat1;
    double deltalon = ToRadians(node2.node->lon - node1.node->lon);
    double a = sin(deltalat / 2) * sin(deltalat / 2) + cos(lat1) * cos(lat2) *
               sin(deltalon / 2) * sin(deltalon / 2);
    double c = 2 * atan2(sqrt(a), sqrt( (1 - a)*(1+a)));

    //double c = 2 * asin(sqrt(a));

    /*double y1 = cos(lat2)*sin(deltalon);
    double y2 = cos(lat1)*cos(lat2)-sin(lat1)*cos(lat2)*cos(deltalon);
    double y = sqrt(y1*y1 + y2*y2);
    double x = sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(deltalon);
    double c = atan2(y,x);  */

    return EARTH_RADIUS * c;
}

AStarNode *NodeWithLowestF(AStarNode *asnode, unsigned long nnodes)
{
    AStarNode *lowfnode = NULL;
    int i;
    for (i = 0; i < nnodes; i++)
    {
        if (asnode[i].stat == OPEN)
        {
            if (lowfnode != NULL)
            {
                if (lowfnode->f > asnode[i].f)
                {
                    lowfnode = &asnode[i];
                }
            }
            else
                lowfnode = &asnode[i];
        }
    }

    if (lowfnode == NULL)
        ExitError("unable to find the lowest f node", 150);

    return lowfnode;
}

void AStar(Node *node, unsigned long nnodes, unsigned long id_start,
           unsigned long id_goal)
{
    AStarNode *asnode, *start_node, *goal_node, *current_node;
    asnode = (AStarNode *) malloc(sizeof(AStarNode) * nnodes);

    // Initiate status of all nodes
    unsigned long i;
    for (i = 0; i < nnodes; i++)
    {
        asnode[i].node = &node[i];
        asnode[i].stat = NOT_VISITED;
    }

    // Search for start and goal nodes
    id_start = BinarySearchChkd(id_start, node, 0, nnodes - 1, 140);
    start_node = &asnode[id_start];
    id_goal = BinarySearchChkd(id_goal, node, 0, nnodes - 1, 141);
    goal_node = &asnode[id_goal];

    // Initialization
    asnode[id_start].stat = OPEN;
    asnode[id_start].g = 0;
    asnode[id_start].f = HeuristicHaversine(asnode[id_start], *goal_node);

    printf("\nlatitude (start): %f longitude (start): %f\nlatitude (goal): %f longitude (goal): %f\n", asnode[id_start].node->lat,asnode[id_start].node->lon,asnode[id_goal].node->lat,asnode[id_goal].node->lon);
    printf("Distance from node start to node goal: %f\n\n", asnode[id_start].f);
    
    //Extra stop condition parameters
    int max_iterations = 10000;
    int current_iteration = 0;

    // Initialization of auxiliary id and node objects for successors
    unsigned long id_successor;
    AStarNOde * successor_node;
    unsigned short current_nsucc;

    while ( AnyOpen(asnode, nnodes) & (current_iteration <= max_iterations) )
    {   
        current_iteration += 1;

        current_node = NodeWithLowestF(asnode, nnodes);
        current_nsucc = current_node->node->nsucc;

        if (current_node->node->id == id_goal)
            break;

        for (i = 0; i < current_nsucc; i++)
        {
            id_successor = BinarySearchChkd(current_node->node->successor[i]->id, node, 0, nnodes - 1, 142);
            successor_node = &asnode[id_successor];





        }

        
        //END A* ITERATION

        if(current_iteration % 100 == 0){
             printf("Finished iteration %d\n", current_iteration);
        }
        //printf("%lu\n", current_node->node->id);
        //asnode[id_start].stat = CLOSE;
    }
    //free(asnode);
}
