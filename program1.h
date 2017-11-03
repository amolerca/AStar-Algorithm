typedef struct {
	unsigned long id; // Node identification
	char *name;
	double lat, lon; // Node position
	unsigned short nsucc; // Number of node successors; i. e. length of successors
	unsigned long *successors;
} node;

