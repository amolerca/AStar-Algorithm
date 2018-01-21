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
#include "test.h"
#include "constants.h"
#include "util.h"
#include "graph.h"
#include "reader.h"
#include "binary.h"

void TestBinWR()
{
    Node *written_node, *read_node;
    unsigned long nnodes, nways, nedges;

    // Write written_node to binary file
    written_node = ReadFile(TEST_MAP_DIR, &nnodes, &nways, &nedges);
    WriteBin(TEST_BIN_DIR, written_node, nnodes);

    // Read read_node from binary file
    read_node = ReadBin(TEST_BIN_DIR, &nnodes);

    printf("\n");
    printf("##############################################################\n");
    printf("Binary writing/reading test starts\n");

    // Print all information of first, medium and last nodes
    printf(" Printing all information of written node %lu\n",
           (unsigned long) 0);
    PrintNodeByIndex(0, written_node, nnodes);
    printf(" Printing all information of read node %lu\n", (unsigned long) 0);
    PrintNodeByIndex(0, read_node, nnodes);
    printf(" Printing all information of written node %lu\n", nnodes / 2 );
    PrintNodeByIndex(nnodes / 2, written_node, nnodes);
    printf(" Printing all information of read node %lu\n", nnodes / 2);
    PrintNodeByIndex(nnodes / 2, read_node, nnodes);
    printf(" Printing all information of written node %lu\n", nnodes - 1);
    PrintNodeByIndex(nnodes -1, written_node, nnodes);
    printf(" Printing all information of read node %lu\n", nnodes -1);
    PrintNodeByIndex(nnodes - 1, read_node, nnodes);

    // Compare both written_node and read_node graphs
    unsigned long i, j;
    unsigned long problems = 0;
    printf(" Comparing all nodes...\n");
    for (i = 0; i < nnodes; i++)
    {
        // Check ids
        if (written_node[i].id != read_node[i].id)
        {
            printf("Problem! Ids in written node %lu and read node %lu does "
                   "not match\n", written_node[i].id, read_node[i].id);
            problems++;
            if (problems == 100) break;
        }
        if (!EqualStrings(written_node[i].name, read_node[i].name))
        {
            printf("Problem! Names in written node %lu and read node %lu does "
                   "not match\n", written_node[i].id, read_node[i].id);
            problems++;
            if (problems == 100) break;
        }
        if (written_node[i].lat != read_node[i].lat)
        {
            printf("Problem! Latitudes in written node %lu and read node %lu "
                   "does not match\n", written_node[i].id, read_node[i].id);
            problems++;
            if (problems == 100) break;
        }
        if (written_node[i].lon != read_node[i].lon)
        {
            printf("Problem! Longitudes in written node %lu and read node %lu "
                   "does not match\n", written_node[i].id, read_node[i].id);
            problems++;
            if (problems == 100) break;
        }
        if (written_node[i].nsucc != read_node[i].nsucc)
        {
            printf("Problem! Number of successors in written node %lu and read"
                   " node %lu does not match\n", written_node[i].id,
                   read_node[i].id);
            problems++;
            if (problems == 100) break;
        }
        if (written_node[i].asucc != read_node[i].asucc)
        {
            printf("Problem! Number of allocated successors in written node "
                   "%lu and read node %lu does not match\n",
                   written_node[i].id, read_node[i].id);
            problems++;
            if (problems == 100) break;
        }
        for (j = 0; j < written_node[i].nsucc; j++)
            if (written_node[i].successor[j]->id !=
                read_node[i].successor[j]->id)
            {
                printf("Problem! Successors in written node %lu and read"
                       " node %lu does not match\n", written_node[i].id,
                       read_node[i].id);
                problems++;
                break;
            }
        if (problems == 100) break;
    }

    if (problems == 100)
        printf("Showing only the first 100 problems found\n");

    printf("Number of problems found: %lu\n", problems);
    printf("Binary writing/reading test ended\n");
    printf("##############################################################\n");
}

void TestEnh()
{
    Node *ori_node, *enh_node;
    unsigned long ori_nnodes, enh_nnodes, nways, nedges;

    // Get original graph
    ori_node = ReadFile(TEST_MAP_DIR, &ori_nnodes, &nways, &nedges);

    // Enhance it
    enh_nnodes = ori_nnodes;
    enh_node = GraphEnhancement(ori_node, &enh_nnodes, nways, nedges);

    // Get original graph again (it was set free)
    ori_node = ReadFile(TEST_MAP_DIR, &ori_nnodes, &nways, &nedges);

    printf("\n");
    printf("##############################################################\n");
    printf("Graph enhancement test starts\n");

    // Compare lengths
    unsigned long dif_nnodes = ori_nnodes - enh_nnodes;
    printf("Original nnodes: %lu\n", ori_nnodes);
    printf("Enhanced nnodes: %lu\n", enh_nnodes);
    printf("Difference:      %lu\n", dif_nnodes);

    // Print all information of first, medium and last nodes
    printf(" Printing all information of original node %lu\n",
           (unsigned long) 0);
    PrintNodeByIndex(0, ori_node, ori_nnodes);
    printf(" Printing all information of enhanced node %lu\n",
           (unsigned long) 0);
    PrintNodeByIndex(0, enh_node, enh_nnodes);
    printf(" Printing all information of written node %lu\n", ori_nnodes - 1);
    PrintNodeByIndex(ori_nnodes -1, ori_node, ori_nnodes);
    printf(" Printing all information of read node %lu\n", enh_nnodes -1);
    PrintNodeByIndex(enh_nnodes - 1, enh_node, enh_nnodes);

    printf("Binary writing/reading test ended\n");
    printf("##############################################################\n");
}

// Main function
int main(int argc, char *argv[])
{
    const char *test_type;
    test_type = SingleArgumentParser(argc, argv);
    if (test_type == NULL)
        ExitError("test type needs to be introduced as a command-line argument"
                  , 50);

    if      (EqualStrings("binary_writing_reading", test_type) ||
             EqualStrings("bwr", test_type)                      )
        TestBinWR();

    else if (EqualStrings("graph_enhancement", test_type)      ||
             EqualStrings("ge", test_type)                       )
        TestEnh();

    else ExitError("an invalid test type was chosen", 51);
}
