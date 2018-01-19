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
    int i;
    for (i = 0; i < nnodes - 1; i++)
        if (node[i].id >= node[i + 1].id)
            return false;
    return true;
}

/*
void InsertNode(Node *node, Node **nodes_list, unsigned long *ln_size)
{
    long search_results;
    unsigned long index;
    search_results = BinarySearch(id, node, 0, nnodes - 1);
    if (search_results == -1){
        return NULL;
    }
    index = (unsigned long) search_results;

}*/

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges)
{
    int i, j;
    unsigned long ln_size;
    unsigned long id;

    printf(" Reindexing nodes...\n");
    for (i = 0; i < *nnodes; i++)
        node[i].id = i;

    char *linked_nodes = (char *) malloc(sizeof(char) * (*nnodes));
    memset(linked_nodes, false, sizeof(char) * *nnodes);

    for (i = 0; i < *nnodes; i++)
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

    // Get the number of unlinked nodes
    unsigned long n_del = 0;
    for (i = 0; i < *nnodes; i++)
        if (linked_nodes[i] == 0)
            n_del++;

    if (n_del > 0)
    {
        printf(" Total number of unlinked nodes found: %lu\n", n_del);
        *nnodes -= n_del;
        Node *enh_node = (Node *) malloc(sizeof(Node) * (*nnodes));
        if (enh_node == NULL)
            ExitError("when allocating memory to save the enhanced graph", 6);

        printf(" Removing unlinked nodes...\n");
        unsigned long enh_node_index = 0;
        for (i = 0; i < *nnodes; i++)
            if (linked_nodes[i] == 1)
            {
                enh_node[enh_node_index] = node[i];
                enh_node_index++;
            }

        printf(" Reindexing nodes...\n");
        for (i = 0; i < *nnodes; i++)
            node[i].id = i;

        // Reallocate memory to save all nodes
        printf(" Reallocating memory to save the enhanced graph...\n");
        free(node);
        node = (Node *) malloc(sizeof(Node) * (*nnodes));
        if (enh_node == NULL)
            ExitError("when allocating memory to save the enhanced graph", 6);

        // Copying enhanced graph
        for (i = 0; i < *nnodes; i++)
            node[i] = enh_node[i];
        free(linked_nodes);
        free(enh_node);

        // Display new graph size
        printf("New graph size: \n");
        printf(" Number of nodes: %lu\n", *nnodes);
        printf(" Number of ways: %lu\n", nways);
        printf(" Number of edges: %lu\n", nedges);
    }
    else
        printf(" No unlinked nodes were found\n");

    return node;
}
