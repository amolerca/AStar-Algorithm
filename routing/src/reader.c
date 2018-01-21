#include "routing.h"
#include "constants.h"
#include "util.h"
#include "graph.h"
#include "reader.h"

void Counter(FILE *f, unsigned long *nnodes, unsigned long *nways,
             unsigned long *nedges)
{
    char *line = NULL;
    size_t line_size = INPUT_MAX_CHARS_PER_LINE;
    size_t len;
    char *field;
    int fn;

    // Set counters to zero
    *nnodes = 0;
    *nways  = 0;
    *nedges = 0;

    // Read first line
    len = getline(&line, &line_size, f);

    // Count the number of lines starting by a specific line_type string
    while (len != -1)
    {
        field = SplitFields(line, INPUT_FIELD_DELIMITORS);
        if (StartsWith("n", field))
            (*nnodes)++;
        else if (StartsWith("w", field))
        {
            fn = 0;
            (*nways)++;
            while(field)
            {
                if (fn > WAY_FIELD_NODES)
                    (*nedges)++;
                field = SplitFields(NULL, INPUT_FIELD_DELIMITORS);
                fn++;
            }
        }
        len = getline(&line, &line_size, f);
    }

    // Set the file again to the beginning
    rewind(f);
}

Node GetNodeFromFields(char *fields)
{
    int fn = 0;
    Node node;

    while(fields)
    {
        char *value = strdup(fields);
        float num = atof(value);

        // Get node ID
        if (fn == NODE_FIELD_ID)
            node.id = atol(fields);

        // Get node NAME
        else if (fn == NODE_FIELD_NAME)
            node.name = strdup(fields);

        // Get node LATITUDE
        else if (fn == NODE_FIELD_LAT)
            node.lat = atof(fields);

        // Get node LONGITUDE
        else if (fn == NODE_FIELD_LON)
            node.lon = atof(fields);

        fields = SplitFields(NULL, INPUT_FIELD_DELIMITORS);
        fn++;
    }
    //printf("%lu, %s, %f, %f\n", node.id, node.name, node.lat, node.lon);
    node.asucc = node.nsucc = 0;

    return node;
}

void GetSuccessorsFromFields(char *fields, Node *node, unsigned long nnodes)
{
    int fn      = 0;
    bool oneway = false;
    unsigned long first_id, second_id;
    Node *first_node, *second_node;

    while(fields)
    {
        if (fn == WAY_FIELD_ONEWAY)
        {
            if (StartsWith("o", fields)) // Looking at the 1st letter is enough
                oneway = !oneway;
        }

        else if (fn == WAY_FIELD_NODES)
        {
            // Check if the current field contains an ID
            if (StartsWith("\n", fields))
                break;

            while(fields)
            {
                // Get first ID
                first_id = atol(fields);
                first_node = FromIdToNode(first_id, node, nnodes);

                // Go to next field and check if it contains an ID
                fields = SplitFields(NULL, INPUT_FIELD_DELIMITORS);
                if (fields == NULL)
                    break;

                // Get second ID
                second_id = atol(fields);
                second_node = FromIdToNode(second_id, node, nnodes);

                // Add relation
                if (first_node != NULL && second_node != NULL)
                {
                    AddSuccessor(first_node, second_node);
                    if(!oneway)
                        AddSuccessor(second_node, first_node);
                }
            }
        }

        fields = SplitFields(NULL, INPUT_FIELD_DELIMITORS);
        fn++;
    }
}

void FileParser(FILE *f, char line_type[], Node *node, unsigned long nnodes)
{
    char *line = NULL;
    size_t line_size = INPUT_MAX_CHARS_PER_LINE;
    size_t len;
    unsigned long index = 0;
    char *fields;
    char *first_field;

    len = getline(&line, &line_size, f);
    while (len != -1)
    {
        // Ignore lines starting with '#'
        if (StartsWith(INPUT_LINE_IGNORE, line))
        {
            len = getline(&line, &line_size, f);
            continue;
        }

        fields = SplitFields(line, INPUT_FIELD_DELIMITORS);
        first_field = strdup(fields);

        if (StartsWith(line_type, first_field))
        {
            if (StartsWith(line_type, "node"))
            {
                node[index] = GetNodeFromFields(fields);
                index++;
            }
            else if (StartsWith(line_type, "way"))
                GetSuccessorsFromFields(fields, node, nnodes);
        }

        len = getline(&line, &line_size, f);
    }

    // Set the file again to the beginning
    rewind(f);
}

Node *ReadFile(const char file_dir[], unsigned long *nnodes,
               unsigned long *nways, unsigned long *nedges)
{
    // Let user know what we are doing
    printf("Reading map file \'%s\'...\n", TEST_MAP_DIR);

    // Open file
    FILE *f;
    f = OpenFile(file_dir, "r", 32);

    // Get the total number of nodes and ways from the input file
    printf("Determining graph size...\n");
    Counter(f, nnodes, nways, nedges);
    printf(" Number of nodes to read: %lu\n", *nnodes);
    printf(" Number of ways to read: %lu\n", *nways);
    printf(" Number of edges to establish: %lu\n", *nedges);

    // Allocate memory to save all nodes
    printf("Allocating memory to save the graph...\n");
    Node *node = (Node *) malloc(sizeof(Node) * (*nnodes));
    if (node == NULL)
        ExitError("when allocating memory to save the graph", 5);

    // Read and parse nodes (line by line)
    printf("Parsing data from file...\n Registering nodes...\n");
    FileParser(f, "n", node, *nnodes);

    // Check if node ids were sorted in map file
    printf(" Checking nodes...\n");
    if (!CheckNodes(node, *nnodes))
            ExitError("when checking nodes. Nodes need to be previously "
                      "sorted in map file.\n", 57);

    // Link nodes according to ways
    printf(" Stablishing edges...\n");
    FileParser(f, "w", node, *nnodes);

    // Close input map file
    fclose(f);

    return node;
}
