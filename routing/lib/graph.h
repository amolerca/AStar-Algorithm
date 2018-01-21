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

void PrintNodeByIndex(unsigned long id, Node *node, unsigned long nnodes);

void PrintNodeById(unsigned long id, Node *node, unsigned long nnodes);

bool IsIndexed(Node *node, unsigned long nnodes);

long BinarySearch(unsigned long id, Node *node, unsigned long left,
                  unsigned long right);

void AddSuccessor(Node *A, Node *B);

Node *FromIdToNode(unsigned long id, Node *node, unsigned long nnodes);

char *GetLinkedNodes(Node *node, unsigned long nnodes);

Node *CleanGraph(Node *node, char *linked_nodes, unsigned long nnodes,
                 unsigned long n_del);

bool CheckNodes(Node *node, unsigned long nnodes);

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges);
