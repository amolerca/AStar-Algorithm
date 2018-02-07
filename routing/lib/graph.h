/*****************************************************************************/
// Graph resources
/*****************************************************************************/

typedef struct Node Node;
typedef struct AStarNode AStarNode;

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

struct AStarNode
{
    Node *node;                 /* Pointer to a node */
    unsigned char stat;         /* Node status */
    double g;                   /* Path cost from start to current node */
    double h;                   /* Cost from current node to goal node
                                   predicted by Heuristic function */
    AStarNode *parent;          /* Parent of the (current) node*/
};

typedef struct
{
    AStarNode **node;           /* Pointer to node pointers */
    unsigned long length;       /* Current length of the dynamic array */
    unsigned long alloc_len;    /* Allocated length */
} DynamicNodeArray;


typedef double (*Distance_function)(AStarNode node1, AStarNode node2);


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

bool CheckNodes(Node *node, unsigned long nnodes);

char *GetLinkedNodes(Node *node, unsigned long nnodes);

Node *CleanGraph(Node *node, char *linked_nodes, unsigned long nnodes,
                 unsigned long n_del);

Node *GraphEnhancement(Node *node, unsigned long *nnodes, unsigned long nways,
                       unsigned long nedges);

double Mean_Earth_radius(double lat1, double lat2);

double Haversine_distance(AStarNode node1, AStarNode node2);

double Radius_varying_Haversine_distance(AStarNode node1, AStarNode node2);

double Equirectangular_distance(AStarNode node1, AStarNode node2);

double Spherical_law_of_cosines_distance(AStarNode node1, AStarNode node2);

void AppendToDynArray(DynamicNodeArray *array, AStarNode *asnode);

void RemoveFromDynArray(DynamicNodeArray *array, AStarNode *asnode);

AStarNode *NodeWithLowestF(DynamicNodeArray *open_list);

AStarNode **GetRoute(AStarNode *start_node, AStarNode *goal_node);

void PrintSolution(AStarNode **route, AStarNode *goal_node);

void WriteSolution(AStarNode **route, AStarNode *goal_node, char filename[]);

void AStar(Node *node, unsigned long nnodes, unsigned long id_start,
           unsigned long id_goal, double (*heuristic)(AStarNode node1, AStarNode node2),
           double (*edge_weight)(AStarNode node1, AStarNode node2));
