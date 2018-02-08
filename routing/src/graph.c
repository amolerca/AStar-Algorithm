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
    printf("  Node id:            %lu\n", node[index].index);
    printf("  Node name:          %s\n", node[index].name);
    printf("  Node lat:           %f\n", node[index].lat);
    printf("  Node lon:           %f\n", node[index].lon);
    printf("  Node nsucc:         %hu\n", node[index].nsucc);
    printf("  Node asucc:         %hu\n", node[index].nsucc);
    printf("  Node successor ids: ");
    unsigned short i;
    for (i = 0; i < node[index].nsucc; i++)
        printf("%lu ", node[index].successor[i]->id);
    printf("\n");
    printf("  Node successor indexes: ");
    for (i = 0; i < node[index].nsucc; i++)
        printf("%lu ", node[index].successor[i]->index);
    printf("\n");

}

void PrintNodeById(unsigned long id, Node *node, unsigned long nnodes)
{

    long search_results = BinarySearch(id, node, 0, nnodes - 1);
    if (search_results == -1)
        ExitError("when printing node. Node id not found", 67);
    id = (unsigned long) search_results;

    printf("  Node id:            %lu\n", node[id].id);
    printf("  Node name:          %s\n", node[id].name);
    printf("  Node lat:           %f\n", node[id].lat);
    printf("  Node lon:           %f\n", node[id].lon);
    printf("  Node nsucc:         %hu\n", node[id].nsucc);
    printf("  Node asucc:         %hu\n", node[id].nsucc);
    printf("  Node successor ids: ");
    unsigned short i;
    for (i = 0; i < node[id].nsucc; i++)
        printf("%lu ", node[id].successor[i]->id);
    printf("\n");
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
    unsigned long i, index;
    unsigned short j;
    char *linked_nodes = (char *) malloc(sizeof(char) * nnodes);
    memset(linked_nodes, false, sizeof(char) * nnodes);

    for (i = 0; i < nnodes; i++)
    {
        if (node[i].nsucc > 0)
        {
            linked_nodes[i] = true;
            for (j = 0; j < node[i].nsucc; j++)
            {
                index = node[i].successor[j]->index;
                linked_nodes[index] = true;
            }
        }
    }

    return linked_nodes;
}

void CopyNode(Node *original, Node *copy)
{
    // Copy ID
    copy->id = original->id;

    // Copy name
    if (copy->name != NULL)
    {
        free(copy->name);
        copy->name = NULL;
    }
    if (original->name == NULL)
        copy->name = NULL;
    else
    {
        copy->name = (char *) malloc(sizeof(char) * (strlen(original->name) +
                                                    1));
        strcpy(copy->name, original->name);
    }

    // Copy coordinates
    copy->lat = original->lat;
    copy->lon = original->lon;

    // Copy successors
    unsigned short i;
    copy->nsucc = copy->asucc = original->nsucc;
    if (copy->successor != NULL)
    {
        free(copy->successor);
        copy->successor = NULL;
    }
    if (original->successor != NULL)
    {
        copy->successor = (Node **) malloc(sizeof(Node *) * copy->nsucc);
        for (i = 0; i < copy->nsucc; i++)
            copy->successor = original->successor;
    }
    else
        copy->successor = NULL;
}

void RemoveNode(Node *node)
{
    node->id = 0;
    if (node->name != NULL)
    {
        free(node->name);
        node->name = NULL;
    }
    if (node->nsucc > 0)
    {
        free(node->successor);
        node->successor = NULL;
    }
}

Node *CleanGraph(Node *node, char *linked_nodes, unsigned long nnodes,
                 unsigned long n_del, unsigned long nsucc)
{
    unsigned long i;

    // Get a list with all successors indexes
    printf(" - Saving links...\n");
    unsigned long *ids;
    ids = (unsigned long *) malloc(sizeof(unsigned long) * nsucc);
    if (ids == NULL)
        ExitError("when allocating memory to save successors to binary"
                  "file", 6);

    // Save node links
    unsigned short j;
    unsigned long index = 0;
    for (i = 0; i < nnodes + n_del; i++)
    {
        for (j = 0; j < node[i].nsucc; j++)
        {
            ids[index] = node[i].successor[j]->id;
            index++;
        }
        // No need to copy original successors because we are still moving
        // nodes around. Since we have already saved node links, we can free
        // successor pointers
        if (node[i].successor != NULL)
        {
            free(node[i].successor);
            node[i].successor = NULL;
        }
    }

    // Add nodes (only the linked ones)
    printf(" - Moving nodes...\n");
    index = 0;
    for (i = 0; i < nnodes + n_del; i++)
    {
        if (linked_nodes[i] == 1)
        {
            if (i != index)
            {
                CopyNode(&node[i], &node[index]);
                RemoveNode(&node[i]);
            }
            index++;
        }
        else
            RemoveNode(&node[i]);
    }

    // Relink nodes
    printf(" - Relinking nodes...\n");
    index = 0;
    unsigned long id_successor;
    Node *successor;
    for (i = 0; i < nnodes; i++)
    {
        node[i].asucc = node[i].nsucc;
        if(node[i].asucc)
        {
            node[i].nsucc = 0;
            node[i].successor = (Node **) malloc(sizeof(Node *) *
                                                 node[i].asucc);
            for (j = 0; j < node[i].asucc; j++)
            {
                id_successor = ids[index];
                successor = FromIdToNode(id_successor, node, nnodes);
                AddSuccessor(&node[i], successor);
                index++;
            }
        }
    }

    // Reindex new nodes
    printf(" - Reindexing nodes...\n");
    for (i = 0; i < nnodes; i++)
        node[i].index = i;

    // Free memory
    free(ids);

    // Return new nodes
    return node;
}

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges)
{
    //Timing the program
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    // Let user know waht we are doing
    printf("------------------------------------------------------------\n");
    printf("Minimizing graph inconsistencies...\n");
    printf("------------------------------------------------------------\n");

    // Get number of successors
    unsigned long i, nsucc = 0;
    for (i = 0; i < *nnodes; i++)
        nsucc += node[i].nsucc;
    printf(" - Total number of succesors found: %lu\n", nsucc);

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
        printf(" - Total number of unlinked nodes found: %lu\n", n_del);

        // Substract the number of nodes that will be removed
        *nnodes -= n_del;

        // Copying a clean graph to enh_node
        printf(" - Removing unlinked nodes...\n");
        node = CleanGraph(node, linked_nodes, *nnodes, n_del, nsucc);

        // Display new graph size
        printf(" - New graph size: \n");
        printf("  - Number of nodes: %lu\n", *nnodes);
        printf("  - Number of successors: %lu\n", nsucc);
        printf("  - Number of ways: %lu\n", nways);
        printf("  - Number of edges: %lu\n", nedges);

        // End timing
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        printf("------------------------------------------------------------"
               "\n");
        printf("Completed in %.2f CPU seconds.\n", cpu_time_used);
        printf("------------------------------------------------------------"
               "\n\n");
    }
    else
        printf(" - No unlinked nodes were found\n");

    free(linked_nodes);

    return node;
}

double MeanEarthRadius(double lat1, double lat2){
    /*
        Computes the mean earth radius between two nodes
        using the geocentric radius formula.
    */
    #define a EARTH_EQUATORIAL_RADIUS
    #define b EARTH_POLAR_RADIUS

    double R1 = sqrt(((a * a * cos(lat1)) * (a * a * cos(lat1)) +
                      (b * b * sin(lat1)) * (b * b * sin(lat1)))
                     / ((a * cos(lat1)) * (a * cos(lat1)) +
                        (b * sin(lat1)) * (b * sin(lat1))));

    double R2 = sqrt(((a * a * cos(lat2)) * (a * a * cos(lat2)) +
                      (b * b * sin(lat2)) * (b * b * sin(lat2)))
                     / ((a * cos(lat2)) * (a * cos(lat2)) +
                        (b * sin(lat2)) * (b * sin(lat2))));

    double R = (R1 + R2) / 2.0;

    #undef a
    #undef b

    return R;
}

double HaversineDistance(AStarNode node1, AStarNode node2)
{
    double lat1 = ToRadians(node1.node->lat);
    double lat2 = ToRadians(node2.node->lat);
    double deltalat = lat2 - lat1;
    double deltalon = ToRadians(node2.node->lon - node1.node->lon);
    double a = sin(deltalat / 2) * sin(deltalat / 2) + cos(lat1) * cos(lat2) *
               sin(deltalon / 2) * sin(deltalon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return EARTH_RADIUS * c;
}

// RVH stands for Radius Varying Haversine
double RVHDistance(AStarNode node1, AStarNode node2)
{
    double lat1 = ToRadians(node1.node->lat);
    double lat2 = ToRadians(node2.node->lat);
    double deltalat = lat2 - lat1;
    double deltalon = ToRadians(node2.node->lon - node1.node->lon);
    double a = sin(deltalat / 2) * sin(deltalat / 2) + cos(lat1) * cos(lat2) *
               sin(deltalon / 2) * sin(deltalon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return MeanEarthRadius(lat1, lat2) * c;
}

double EquirectangularDistance(AStarNode node1, AStarNode node2)
{
    double lat1 = ToRadians(node1.node->lat);
    double lat2 = ToRadians(node2.node->lat);
    double deltalat = lat2 - lat1;
    double mean_lat = (lat1+lat2)/2.0;
    double deltalon = ToRadians(node2.node->lon - node1.node->lon);

    double x = deltalon * cos(mean_lat);
    double y = deltalat;

    double c = sqrt(x*x + y*y);

    return EARTH_RADIUS * c;
}

// SLOC stands for Spherical Lay of Cosines
double SLOCDistance(AStarNode node1, AStarNode node2)
{
    double lat1 = ToRadians(node1.node->lat);
    double lat2 = ToRadians(node2.node->lat);
    double deltalat = lat2 - lat1;
    double deltalon = ToRadians(node2.node->lon - node1.node->lon);

    double c = acos(sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(deltalon));

    return EARTH_RADIUS * c;
}

double ZeroDistance(AStarNode node1, AStarNode node2)
{
    return 0.0;
}

double UniformDistance(AStarNode node1, AStarNode node2)
{
    return 1.0;
}

void AppendToDynArray(DynamicNodeArray *array, AStarNode *asnode)
{
    // Append node to dynamic array
    if (array->length == array->alloc_len)
    {
        array->alloc_len += DYN_REALLOC_SIZE;
        array->node = (AStarNode **) realloc(array->node,
                                            sizeof(AStarNode *) *
                                            (array->alloc_len));
    }
    array->node[array->length] = asnode;
    asnode->index = array->length;

    // Update dynamic array's length
    array->length++;

    // Update node's status
    asnode->stat = OPEN;
}

void RemoveFromDynArray(DynamicNodeArray *array, AStarNode *asnode)
{
    // Update dynamic array's length
    array->length--;

    // Copy last element of the array to the position of the node that we want
    // to remove
    array->node[asnode->index] = array->node[array->length];

    // Update index of the node that has been moved
    array->node[asnode->index]->index = asnode->index;

    // Update node's status
    asnode->stat = CLOSE;
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

AStarNode **GetRoute(AStarNode *start_node, AStarNode *goal_node)
{
    // Let user know what we are doing
    printf("Getting route from AStar results...\n");

    unsigned int route_size = ROUTE_REALLOC_SIZE;
    AStarNode **inverted_route = (AStarNode **) malloc(sizeof(AStarNode *) *
                                                       route_size);
    unsigned int i = 0, j;
    inverted_route[i] = goal_node;

    while (inverted_route[i]->node->id != start_node->node->id)
    {
        if (i == route_size - 1)
        {
            route_size += ROUTE_REALLOC_SIZE;
            inverted_route = realloc(inverted_route, sizeof(AStarNode *) *
                                     route_size);
        }
        i++;
        inverted_route[i] = inverted_route[i-1]->parent;
    }

    AStarNode **route = (AStarNode **) malloc(sizeof(AStarNode *) * (i + 1));

    for (j = 0; j < i + 1; j++)
        route[j] = inverted_route[i - j];

    return route;
}

void PrintSolution(AStarNode **route, AStarNode *goal_node)
{
    printf("Route summary:\n");
    printf(" Starting node: %lu %s\n", route[0]->node->id,
           route[0]->node->name);
    printf(" Goal node: %lu %s\n", goal_node->node->id, goal_node->node->name);
    printf(" Distance: %f\n", goal_node->h + goal_node->g);
    printf(" Path summary:\n");

    printf("%10d | %10lu | % 6.5f | %6.5f | %s \n", 1,
           route[0]->node->id, route[0]->node->lat, route[0]->node->lon,
           route[0]->node->name);

    unsigned int i = 1;
    while (route[++i]->node->id != goal_node->node->id)
    {
        if (route[i]->node->name == NULL)
            continue;
        printf("%10d | %10lu | % 6.5f | % 6.5f | %s \n", i + 1,
               route[i]->node->id, route[i]->node->lat, route[i]->node->lon,
               route[i]->node->name);
    }

    printf("%10d | %10lu | % 6.5f | % 6.5f | %s \n", i + 1,
           route[i]->node->id, route[i]->node->lat, route[i]->node->lon,
           route[i]->node->name);
}

void WriteSolution(AStarNode **route, AStarNode *goal_node, char filename[])
{
    FILE *file = OpenFile(filename, "wb", 418);

    fprintf(file, "# ROUTE RESULTS\n");
    fprintf(file, "# This file is written with the purpose to be easily read "
            "by plot_route.py. Read README.md file to get more information "
            "about it\n");
    fprintf(file, "# Fields arrangement:\n");
    fprintf(file, "# Node number | Node ID | Node longitude | Node latitude | "
            "Node longitude | Node name\n");

    unsigned int i = 0;
    fprintf(file, "%10d | %10lu | % 6.5f | % 6.5f | %s \n", i + 1,
            route[i]->node->id, route[i]->node->lat, route[i]->node->lon,
            route[i]->node->name);
    while (route[i++]->node->id != goal_node->node->id)
        fprintf(file, "%10d | %10lu | % 6.5f | % 6.5f | %s \n", i + 1,
                route[i]->node->id, route[i]->node->lat, route[i]->node->lon,
                route[i]->node->name);
}

dist_function SelDistFunction(char query[])
{
    unsigned int choice = MakeAQuery(query, 1, 6);

    dist_function chosen_function;
    if (choice == 1)
        chosen_function = &HaversineDistance;
    else if (choice == 2)
        chosen_function = &SLOCDistance;
    else if (choice == 3)
        chosen_function = &EquirectangularDistance;
    else if (choice == 4)
        chosen_function = &RVHDistance;
    else if (choice == 5)
        chosen_function = &ZeroDistance;
    else if (choice == 6)
        chosen_function = &UniformDistance;
    else
        ExitError("wrong user selection", 514);

    return chosen_function;
}

void AStar(Node *node, unsigned long nnodes, unsigned long id_start,
           unsigned long id_goal)
{
    AStarWelcome();
    PrintOutDistOptions();

    dist_function heuristic = SelDistFunction("Choose a method to compute the "
                                              "heuristic distance");
    dist_function edge_weight = SelDistFunction("Choose a method to compute "
                                                "the weight between edges");

     //Timing the AStar implementation 
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    // Let user know that AStar algorithm is starting
    printf(" - Calculating route with AStar algorithm...\n\n");

    // Initiate variables before starting the algorithm
    AStarNode *asnode, *start_node, *goal_node, *current_node, *successor_node;
    asnode = (AStarNode *) malloc(sizeof(AStarNode) * nnodes);
    unsigned short j;
    unsigned long succ_ID;
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
        asnode[i].index = -1;
        asnode[i].node = &node[i];
        asnode[i].stat = NOT_VISITED;
    }

    // Search for start and goal nodes
    id_start = BinarySearchChkd(id_start, node, 0, nnodes - 1, 140);
    start_node = &asnode[id_start];
    id_goal = BinarySearchChkd(id_goal, node, 0, nnodes - 1, 141);
    goal_node = &asnode[id_goal];

    // AStar initialization
    AppendToDynArray(open_list, start_node);
    start_node->g = 0;
    start_node->h = (*heuristic)(*start_node, *goal_node);

    // Initiate AStar algorithm
    unsigned int current_iteration = 0;
    while (open_list->length != 0)
    {
        // Sum up one iteration
        current_iteration += 1;

        // Get the Node with lowest f(n) = g(h) + h(n), where g(h) is the cost
        // of the path from starting node to current node and h(n) is the cost
        // from the current node to the goal node which is calculated with the
        // heuristic function
        current_node = NodeWithLowestF(open_list);
        //printf("iteration %u\n", current_iteration);
        //printf("id %lu\n", current_node->node->id);
        //printf("nsucc %hu\n", current_node->node->nsucc);

        // Finish loop in case of finding a solution
        if (current_node->node->id == goal_node->node->id)
            break;

        // Find all the successors of the current node
        for (j = 0; j < current_node->node->nsucc; j++)
        {
            // Get successor's ID
            succ_ID = current_node->node->successor[j]->index;
            //printf("succ_ID %lu\n", succ_ID);

            // Find successor's node
            successor_node = &asnode[succ_ID];

            // Calculate successor's cost according to the current path
            successor_current_cost = current_node->g +
                                     (*edge_weight)(*current_node,
                                                        *successor_node);
            //printf("%lu %i\n", successor_node->node->id, successor_node->stat);

            // Depending on whether the successor's node was previously visited
            // or not, perform a specific action
            if (successor_node->stat == OPEN)
            {
                // If successor's node was previously visited and it is still
                // open, update its status if and only if its former cost is
                // higher than the current one
                if(successor_node->g <= successor_current_cost)
                    continue;
            }
            else if (successor_node->stat == CLOSE)
            {
                // If successor's node was previously visited and now it is
                // closed, update its status and place it in OPEN list if and
                // only if its former cost is higher than the current one
                if(successor_node->g <= successor_current_cost)
                    continue;
                AppendToDynArray(open_list, successor_node);
            }
            else
            {
                // If successor's node has not been visited yet, update its
                // status, place it in OPEN list and calculate its heuristic
                // cost (since it is the first time for this node to be read)
                AppendToDynArray(open_list, successor_node);
                successor_node->h = (*heuristic)(*successor_node,
                                                       *goal_node);
            }

            // The status of those nodes that satisfied one of the previous
            // conditions are updated
            successor_node->g = successor_current_cost;
            successor_node->parent = current_node;
        }

        // We remove the current node from the OPEN list
        RemoveFromDynArray(open_list, current_node);

        // Print useful information in a certain time interval
        if (current_iteration % 200000 == 0)
            PrintOutIterationInfo(current_iteration, current_node->g,
                                  current_node->h);
    }

    // Check for errors
    if (current_node->node->id != goal_node->node->id)
        ExitError("when finishing A* algorithm. OPEN list is empty and no "
                  "solution was found", 460);

    // End timing
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Notify success
    PrintOutResults(current_iteration, current_node->g, current_node->h, cpu_time_used);
    
    printf("Do you want to print a summary of the optimal path found and to save the completed one in a file?\n");
    if(ParseYesNo()){
        // Get route from AStar nodes
        AStarNode **route = GetRoute(start_node, goal_node);

        // Print results and save route
        PrintSolution(route, goal_node);
        WriteSolution(route, goal_node, "routes/path1.out");
    }
}
