#include "routing.h"
#include "constants.h"
#include "util.h"
#include "graph.h"

unsigned long CountLines(char line_type[], FILE *f)
{
    char *line = NULL;
    size_t line_size = INPUT_MAX_CHARS_PER_LINE;
    size_t len;
    char *first_field;

    len = getline(&line, &line_size, f);

    // Set the counter to zero
    unsigned long counter = 0;

    // Count the number of lines starting by a specific line_type string
    while (len != -1)
    {
        first_field = SplitFields(line, INPUT_FIELD_DELIMITORS);
        if (StartsWith(first_field, line_type))
            counter++;
        len = getline(&line, &line_size, f);
    }

    // Set the file again to the beginning
    rewind(f);

    // Return the number of matchs
    return counter;
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

FILE *OpenFile(char file_dir[])
{
    FILE *f;

    f = fopen(file_dir, "r");

    if (f == NULL)
        ExitError("when reading the input map file", 32);

    return f;

}

void ReadFile(char file_dir[])
{
    FILE *f;

    f = OpenFile(file_dir);


    //len = getline(&line, &len, f);
    //printf("Retrieved line of length %zu:\n", len);
    //printf("%s", line);

    // Determine map size
    printf("Determining map size...\n");

    // Get the total number of nodes from the input file
    unsigned long nnodes = CountLines("node", f);

    // Get the total number of ways from the input file (useless)
    unsigned long nways = CountLines("way", f);

    // Print information about map file
    printf(" Number of nodes to read: %lu\n", nnodes);
    printf(" Number of ways to read: %lu\n", nways);

    // Allocate memory to save all nodes
    printf("Allocating memory to save all nodes...\n");
    Node *node = (Node *) malloc(sizeof(Node) * nnodes);
    if (node == NULL)
        ExitError("when allocating memory to save the nodes", 5);

    // Read and parse the file (line by line)
    printf("Parsing data from file...\n");
    printf(" Registering nodes...\n");
    FileParser(f, "n", node, nnodes);
    printf(" Linking ways...\n");
    FileParser(f, "w", node, nnodes);

    printf("%lu\n", node[nnodes-1].id);
    Node *index1 = FromIdToNode(8670492, node, nnodes);
    if (index1 == NULL)
        printf("NULL!\n");
    else
        printf("ID: %lu\n", index1->id);

    Node *index = FromIdToNode(30227456, node, nnodes);
    if (index == NULL)
        printf("NULL!\n");
    else
        printf("ID: %lu\n", index->id);
    printf("%lu:", node[0].id);
    for (int i = 0; i < node[0].nsucc; i++)
        printf(" %lu", node[0].successor[i]->id);
    printf("\n");
}
