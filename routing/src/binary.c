#include "routing.h"
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

     //Timing the program
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    // Let user know what we are doing
    printf("------------------------------------------------------------\n");
    printf("Writting graph to binary file \'%s\'...\n", bin_dir);
    printf("------------------------------------------------------------\n");

    // Open file
    FILE *f;
    f = OpenFile(bin_dir, "wb", 33);

    // Write total number of nodes
    printf(" - Writting number of nodes and successors...\n");
    if ((fwrite(&nnodes, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when initiating binary file", 12);

    // Calculate total number of successors
    unsigned long i;
    unsigned long nsucc = 0;
    for (i = 0; i < nnodes; i++)
        nsucc += node[i].nsucc;

    // Write total number of successors
    if ((fwrite(&nsucc, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when initiating binary file", 13);

    // Write all nodes
    printf(" - Writting nodes...\n");
    if ((fwrite(node, sizeof(Node), nnodes, f)) != nnodes)
        ExitError("when writing nodes to binary file", 14);

    // Get a list with all successors indexes
    printf(" - Writting successors...\n");
    unsigned long *indexes;
    indexes = (unsigned long *) malloc(sizeof(unsigned long) * nsucc);
    if (indexes == NULL)
        ExitError("when allocating memory to save successors to binary"
                  "file", 6);

    unsigned short j;
    unsigned long index = 0;
    for (i = 0; i < nnodes; i++)
        for (j = 0; j < node[i].nsucc; j++)
        {
            indexes[index] = node[i].successor[j]->index;
            index++;
        }

    // Write all successors
    if ((fwrite(indexes, sizeof(unsigned long), nsucc, f)) != nsucc)
        ExitError("when writing node successors to binary file", 15);

    // Write node names
    printf(" - Writting node labels...\n");
    unsigned int name_len;
    for (i = 0; i < nnodes; i++)
    {
        if (node[i].name == NULL)
        {
            name_len = 0;
            if ((fwrite(&name_len, sizeof(unsigned int), 1, f)) != 1)
                ExitError("when writing node names", 16);
        }
        else
        {
            name_len = strlen(node[i].name);
            if ((fwrite(&name_len, sizeof(unsigned int), 1, f)) != 1)
                ExitError("when writing node names", 16);
            if ((fwrite(node[i].name, sizeof(char), name_len, f)) != name_len)
                ExitError("when writing node names", 17);
        }
    }
    
    // End timing
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print out success
    printf("------------------------------------------------------------\n");
    printf("Completed in %.2f CPU seconds.\n", cpu_time_used);
    printf("------------------------------------------------------------\n\n");

    // Free memory
    free(indexes);

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
    //Timing the program
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    // Let user know what we are doing
    printf("------------------------------------------------------------\n");
    printf("Reading graph from binary file \'%s\'...\n", bin_dir);
    printf("------------------------------------------------------------\n");
    
    // Open file
    FILE *f;
    f = OpenFile(bin_dir, "r", 34);

    // Read total number of nodes
    printf(" - Reading number of nodes and successors...\n");
    unsigned long nsucc;
    if ((fread(nnodes, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when reading binary file", 70);

    // Read total number of successors
    if ((fread(&nsucc, sizeof(unsigned long), 1, f)) != 1)
        ExitError("when reading binary file", 71);

    // Print number of nodes and succesors that were read
    printf("  - Nodes: %lu\n", *nnodes);
    printf("  - Successors: %lu\n", nsucc);

    // Allocate memory to save the graph
    Node *node = (Node *) malloc(sizeof(Node) * (*nnodes));
    if (node == NULL)
        ExitError("when allocating memory to save the graph", 7);

    // Read nodes
    printf(" - Reading nodes...\n");
    if ((fread(node, sizeof(Node), *nnodes, f)) != *nnodes)
        ExitError("when reading binary file", 72);

    // Check if node ids were sorted in map file
    printf(" Checking nodes...\n");
    if (!CheckNodes(node, *nnodes))
            ExitError("nodes are not sorted in binary file", 58);

    // Allocate memory to save node successor indexes
    unsigned long *indexes;
    indexes = (unsigned long *) malloc(sizeof(unsigned long) * nsucc);
    if (indexes == NULL)
        ExitError("when allocating memory to save the graph", 8);

    // Read successors
    printf(" - Linking nodes...\n");
    if ((fread(indexes, sizeof(unsigned long), nsucc, f)) != nsucc)
        ExitError("when reading binary file", 73);

    // Assign successors
    unsigned long i, index = 0;
    unsigned short j;
    for (i = 0; i < *nnodes; i++)
    {
        if(node[i].nsucc)
        {
            node[i].successor = (Node **) malloc(sizeof(Node *) *
                                                 node[i].nsucc);
            for (j = 0; j < node[i].nsucc; j++)
            {
                node[i].successor[j] = &node[indexes[index]];
                index++;
            }
        }
        else
            node[i].successor = NULL;
    }

    // Read node names
    printf(" - Labeling nodes...\n");
    unsigned int name_len;
    char *node_name;
    for (i = 0; i < *nnodes; i++)
    {
        if ((fread(&name_len, sizeof(unsigned int), 1, f)) != 1)
            ExitError("when node names from binary file", 74);
        if (name_len > 0)
        {
            node_name = (char *) malloc(sizeof(char) * name_len);
            if ((fread(node_name, sizeof(char), name_len, f)) != name_len)
                ExitError("when node names from binary file", 75);
            node[i].name = node_name;
        }
        else
            node[i].name = NULL;
    }

    // End timing
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print out success
    printf("------------------------------------------------------------\n");
    printf("Completed in %.2f CPU seconds.\n", cpu_time_used);
    printf("------------------------------------------------------------\n\n");

    // Free memory
    free(indexes);

    // Close file
    fclose(f);

    // Return read graph
    return node;
}
