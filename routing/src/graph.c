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
//#include "constants.h"
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
                search_results = BinarySearch(id, clean_node, 0, nnodes - 1);
                if (search_results == -1)
                    ExitError("when enhancing graph", 140);

                clean_id = (unsigned long) search_results;
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
