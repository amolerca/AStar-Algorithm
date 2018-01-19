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
//#include "util.h"
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
