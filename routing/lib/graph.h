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

typedef struct
{
    Node *node;                 /* Pointer to a node */
    unsigned char stat;         /* Node status */
    double g;                   /* Path cost from start to current node */
    double f;                   /* f(n) = g(n) + h(n), g(n) is the path cost
                                   from start to node n and h(n) is the
                                   heuristic function at node n */
    Node *parent;               /* Parent of the (current) node*/
} AStarNode;

void PrintNodeByIndex(unsigned long id, Node *node, unsigned long nnodes);

void PrintNodeById(unsigned long id, Node *node, unsigned long nnodes);

bool IsIndexed(Node *node, unsigned long nnodes);

long BinarySearch(unsigned long id, Node *node, unsigned long left,
                  unsigned long right);

unsigned long BinarySearchChkd(unsigned long id, Node *node,
                               unsigned long left, unsigned long right,
                               int errnum);

void AddSuccessor(Node *A, Node *B);

Node *FromIdToNode(unsigned long id, Node *node, unsigned long nnodes);

char *GetLinkedNodes(Node *node, unsigned long nnodes);

Node *CleanGraph(Node *node, char *linked_nodes, unsigned long nnodes,
                 unsigned long n_del);

bool CheckNodes(Node *node, unsigned long nnodes);

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges);

bool AnyOpen(AStarNode *asnode, unsigned long nnodes);

double HeuristicHaversine(AStarNode node1, AStarNode node2);

void AStar(Node *node, unsigned long nnodes, unsigned long id_start,
           unsigned long id_goal);
