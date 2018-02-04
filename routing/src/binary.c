#include "routing.h"
//#include "constants.h"
#include "util.h"
#include "graph.h"
#include "binary.h"

void WriteCmap(const char bin_dir[], Node *node, unsigned long nnodes)
{
    // Write binary file
    WriteBin(bin_dir, node, nnodes);

    // Compress binary file
    Compress(bin_dir);
}

void WriteBin(const char bin_dir[], Node *node, unsigned long nnodes)
{
    // Let user know what we are doing
    printf("Writting graph to binary file \'%s\'...\n", bin_dir);

    // Open file
    FILE *f;
    f = OpenFile(bin_dir, "wb", 33);

    // Write total number of nodes
    if ((fwrite(&nnodes, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when initiating binary file", 12);

    // Calculate total number of successors
    unsigned long i, j;
    unsigned long nsucc = 0;
    for (i = 0; i < nnodes; i++)
        nsucc += node[i].nsucc;

    // Write total number of successors
    if ((fwrite(&nsucc, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when initiating binary file", 13);

    // Write all nodes
    if ((fwrite(node, sizeof(Node), nnodes, f)) != nnodes)
        ExitError("when writing nodes to binary file", 14);

    // Get a list with all successors ids
    unsigned long *ids;
    ids = (unsigned long *) malloc(sizeof(unsigned long) * nsucc);
    if (ids == NULL)
        ExitError("when allocating memory to save successors to binary"
                  "file", 6);

    unsigned long index = 0;
    for (i = 0; i < nnodes; i++)
        for (j = 0; j < node[i].nsucc; j++)
        {
            ids[index] = node[i].successor[j]->id;
            index++;
        }

    // Write all successors
    if ((fwrite(ids, sizeof(unsigned long), nsucc, f)) != nsucc)
        ExitError("when writing node successors to binary file", 15);

    // Free memory
    free(ids);

    // Close file
    fclose(f);
}

Node *ReadCmap(const char bin_dir[], unsigned long *nnodes)
{
    // Read node from binary file
    Node *node = ReadBin(bin_dir, nnodes);

    // Compress binary file
    Compress(bin_dir);

    // Return node
    return node;
}

Node *ReadBin(const char bin_dir[], unsigned long *nnodes)
{
    // Let user know what we are doing
    printf("Reading graph from binary file \'%s\'...\n", bin_dir);

    // Open file
    FILE *f;
    f = OpenFile(bin_dir, "r", 34);

    // Read total number of nodes
    printf(" Reading number of nodes and successors...\n");
    unsigned long nsucc;
    if ((fread(nnodes, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when reading binary file", 70);

    // Read total number of successors
    if ((fread(&nsucc, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when reading binary file", 71);

    // Print number of nodes and succesors that were read
    printf("  Nodes: %lu\n", *nnodes);
    printf("  Successors: %lu\n", nsucc);

    // Allocate memory to save the graph
    Node *node = (Node *) malloc(sizeof(Node) * (*nnodes));
    if (node == NULL)
        ExitError("when allocating memory to save the graph", 7);
    ///////////////////////////////////////////
    //AFEGIT DANI
    /////////////////////////////////////////////
    for(int i = 0; i<(*nnodes) ;i++){
        if( (node[i].name = (char *) malloc(sizeof(char) * 184)) == NULL){
            ExitError("when allocating memory for a node.name in ReadBin from binary.c \n", 184);
        } //184 is the maximum length of name (PDF of the assignment)
    }
    //////////////////////////////////////////////////////////////
    
    // Allocate memory to save node successor ids
    unsigned long *ids;
    ids = (unsigned long *) malloc(sizeof(unsigned long) * nsucc);
    if (ids == NULL)
        ExitError("when allocating memory to save the graph", 8);

    // Read nodes
    printf(" Reading nodes...\n");
    if ((fread(node, sizeof(Node), *nnodes, f)) != *nnodes)
        ExitError("when reading binary file", 72);

    // Check if node ids were sorted in map file
    printf(" Checking nodes...\n");
    if (!CheckNodes(node, *nnodes))
            ExitError("nodes are not sorted in binary file", 58);

    // Read successors
    if ((fread(ids, sizeof(unsigned long), nsucc, f)) != nsucc)
        ExitError("when reading binary file", 73);

    // Check if node ids were indexed (in case of having run GraphEnhancement)
    bool indexed = IsIndexed(node, *nnodes);

    // Assign successors
    unsigned long i, j;
    unsigned long index = 0;
    for (i = 0; i < *nnodes; i++)
        if(node[i].nsucc)
        {
            node[i].successor = (Node **) malloc(sizeof(Node *) *
                                                 node[i].nsucc);
            for (j = 0; j < node[i].nsucc; j++)
            {
                if (indexed)
                    node[i].successor[j] = &node[ids[index]];
                else
                    node[i].successor[j] = FromIdToNode(ids[index], node,
                                                        *nnodes);
                index++;
            }
        }
    printf(" ReadBIn finished...\n");
    // Close file
    fclose(f);

    // Return read graph
    return node;
}
