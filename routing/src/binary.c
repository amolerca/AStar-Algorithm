#include "routing.h"
//#include "constants.h"
#include "util.h"
#include "graph.h"
#include "binary.h"

void WriteBin(char bin_dir[], Node *node, unsigned long nnodes)
{
    FILE *f;
    int i, j;

    f = fopen(bin_dir, "wb");
    if (f == NULL)
        ExitError("when opening binary file to write map data", 11);

    // Write total number of nodes
    if ((fwrite(&nnodes, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when initiating binary file", 12);

    // Calculate total number of successors
    unsigned long nsucc = 0;
    for (i = 0; i < nnodes; i++)
        nsucc += node[i].nsucc;

    // Write total number of successors
    if ((fwrite(&nsucc, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when initiating binary file", 13);

    // Write all nodes
    if ((fwrite(node, sizeof(Node*), nnodes, f)) != nnodes)
        ExitError("when writing nodes to binary file", 14);

    // Write all successors in blocks
    unsigned long *ids;
    for (i = 0; i < nnodes; i++)
    {
        if (node[i].nsucc)
        {
            // Get a list of all successors ids
            ids = (unsigned long *) malloc(sizeof(unsigned long) *
                   node[i].nsucc);
            if (ids == NULL)
                ExitError("when allocating memory to save successors to binary"
                          "file", 6);

            for (j = 0; j < node[i].nsucc; j++)
                ids[j] = node[i].successor[j]->id;

            if ((fwrite(ids, sizeof(unsigned long), node[i].nsucc, f))
                != node[i].nsucc)
                ExitError("when writing node successors to binary file", 15);

            free(ids);
        }


    }
    fclose(f);
}
