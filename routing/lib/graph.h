/*****************************************************************************/
// Graph resources
/*****************************************************************************/

typedef struct Node Node;

struct Node
{
    unsigned long id;          /* Node identification (up to 4294967295) */
    char *name;                /* Pointer to the name of the location */
    double lat;                /* Latitude of the node */
    double lon;                /* Longitude of the node */
    unsigned short nsucc;      /* Number of node successors; i. e. length of
                                  successors (up to 65535) */
    unsigned short asucc;      /* Number of allocated successors */
    Node **successor;          /* Pointer to successor nodes */
};

/*
typedef struct
{


} NodesList
*/

long BinarySearch(unsigned long id, Node *node, unsigned long left,
                  unsigned long right);

void AddSuccessor(Node *A, Node *B);

Node *FromIdToNode(unsigned long id, Node *node, unsigned long nnodes);

bool CheckNodes(Node *node, unsigned long nnodes);

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges);
