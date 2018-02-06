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

double edge_weight(Node node1, Node node2)
{
    double lat1 = ToRadians(node1.lat);
    double lat2 = ToRadians(node2.lat);
    double deltalat = lat2 - lat1;
    double deltalon = ToRadians(node2.lon - node1.lon);
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

void AppendToDynArray(DynamicNodeArray *array, AStarNode *asnode)
{
    //printf("append: ");
    if (array->length == array->alloc_len)
    {
        array->alloc_len += DYN_REALLOC_SIZE;
        array->node = (AStarNode **) realloc(array->node,
                                            sizeof(AStarNode *) *
                                            (array->alloc_len));
    }
    array->node[array->length] = asnode;
    array->length++;
    /*
    printf("%lu, %lu\n", array->length, array->alloc_len);
    for (unsigned long i = 0; i < array->length; i++)
        printf("%lu ", array->node[i]->node->id);
    printf("\n");*/

}

void RemoveFromDynArray(DynamicNodeArray *array, AStarNode *asnode)
{
    //printf("remove: ");
    unsigned long i, j = 0;
    bool found = false;
    for (i = 0; i < array->length; i++)
    {
        if (array->node[i]->node->id == asnode->node->id)
            found = true;
        else
        {
            array->node[j] = array->node[i];
            j++;
        }
    }

    if (!found)
        ExitError("when removing node from dynamic array. Node to remove not"
                  " found", 420);

    //array->node[i] = NULL;
    array->length--;
    /*
    printf("%lu\n", array->length);
    for (i = 0; i < array->length; i++)
        printf("%lu ", array->node[i]->node->id);
    printf("\n");*/
}

AStarNode *NodeWithLowestF(DynamicNodeArray *open_list)
{
    AStarNode *lowfnode = open_list->node[0];
    unsigned long i;

    for (i = 1; i < open_list->length; i++)
        if (lowfnode->g + lowfnode->h >
            open_list->node[i]->g + open_list->node[i]->h)
            lowfnode = open_list->node[i];

    return lowfnode;
}

void PrintSolution(AStarNode *asnode, Node *node, unsigned long nnodes,
                   AStarNode *start_node, AStarNode *goal_node)
{
    printf("Results:\n");
    printf(" Starting node:\n");
    PrintNodeById(start_node->node->id, node, nnodes);
    printf(" Goal node:\n");
    PrintNodeById(goal_node->node->id, node, nnodes);
    printf(" Distance: %f\n", goal_node->h + goal_node->g);
    printf(" Path:\n");

    AStarNode *current_node = goal_node;
    printf("  Node id: %10lu | Distance: %5.0f | Name: %s\n", current_node->node->id, current_node->g, current_node->node->name);
    while (current_node->node->id != start_node->node->id)
    {
        current_node = current_node->parent;
        printf("  Node id: %10lu | Distance: %5.0f | Name: %s\n", current_node->node->id, current_node->g, current_node->node->name);
    }
}

void AStar(Node *node, unsigned long nnodes, unsigned long id_start,
           unsigned long id_goal)
{
    AStarNode *asnode, *start_node, *goal_node, *current_node, *successor_node;
    asnode = (AStarNode *) malloc(sizeof(AStarNode) * nnodes);
    unsigned short j;
    unsigned long id_successor;
    double successor_current_cost;

    // Initialize dynamic array to save OPEN nodes
    DynamicNodeArray array, *open_list;
    open_list = &array;
    open_list->node = (AStarNode **) malloc(sizeof(AStarNode *) *
                                           DYN_REALLOC_SIZE);
    open_list->length = 0;
    open_list->alloc_len = DYN_REALLOC_SIZE;

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

    // AStar initialization
    start_node->stat = OPEN;
    AppendToDynArray(open_list, start_node);
    start_node->g = 0;
    start_node->h = HeuristicHaversine(*start_node, *goal_node);

    printf("\nlatitude (start): %f longitude (start): %f\nlatitude (goal): %f longitude (goal): %f\n", asnode[id_start].node->lat,asnode[id_start].node->lon,asnode[id_goal].node->lat,asnode[id_goal].node->lon);
    printf("Distance from node start to node goal: %f\n\n", start_node->h);

    // @TODO Implement a dynamic array to save all OPEN nodes. It will speed up
    // both the while loop check and the NodeWithLowestF() function
    unsigned int current_iteration = 0;
    printf("%lu\n", open_list->length);
    while (open_list->length != 0)
    {
        current_iteration += 1;

        current_node = NodeWithLowestF(open_list);

        if (current_iteration == 1)
        {
            printf("ID of current node in iteration 1: %lu\n ", current_node->node->id);
            printf("Name current node iteration 1: %s\n", current_node->node->name);
        }

        // Finish loop in case of finding a solution
        if (current_node->node->id == goal_node->node->id)
            break;

        for (j = 0; j < current_node->node->nsucc; j++)
        {
            id_successor = BinarySearchChkd(current_node->node->successor[j]->id, node, 0, nnodes - 1, 201);
            successor_node = &asnode[id_successor];
            successor_current_cost = current_node->g + HeuristicHaversine(*current_node, *successor_node);

            // node_successor is in the OPEN list
            if (successor_node->stat == OPEN)
            {
                if(successor_node->g <= successor_current_cost)
                    continue;
            }

            // node_successor is in the CLOSED list
            else if (successor_node->stat == CLOSE)
            {
                if(successor_node->g <= successor_current_cost)
                    continue;
                // Move node_successor from the CLOSED list to the OPEN list
                AppendToDynArray(open_list, successor_node);
                successor_node->stat = OPEN;
            }

            // node_successor has not been visited so far
            else
            {
                AppendToDynArray(open_list, successor_node);
                successor_node->stat = OPEN; // Add node_successor to the OPEN list
                successor_node->h = HeuristicHaversine(*successor_node, *goal_node);
                    // Set h(node_successor) to be the heuristic distance to node_goal
            }

            // In case that either the successor current cost is lower than the
            // previous one or
            successor_node->g = successor_current_cost; //Set g(node_successor) = successor_current_cost
            successor_node->parent = current_node; //Set the parent of node_successor to node_current
        }

        RemoveFromDynArray(open_list, current_node);
        current_node->stat = CLOSE; // Add node_current to the CLOSED list

         //END A* ITERATION
        if (current_iteration % 100000 == 0)
        {
            printf("Finished iteration %d\n", current_iteration);
            printf("Current node %lu\n", current_node->node->id);
        }

    }

    if (current_node->node->id != goal_node->node->id)
        ExitError("when finishing A* algorithm. OPEN list is empty and no "
                  "solution was found", 460);

    printf("ID of current node in iteration %i: %lu\n ", current_iteration,
           current_node->node->id);
    printf("Optimal path found!\n");

    PrintSolution(asnode, node, nnodes, start_node, goal_node);
}
