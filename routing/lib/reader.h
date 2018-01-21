void Counter(FILE *f, unsigned long *nnodes, unsigned long *nways,
             unsigned long *nlinks);

Node GetNodeFromFields(char *fields);

void GetSuccessorsFromFields(char *fields, Node *node, unsigned long nnodes);

void FileParser(FILE *f, char line_type[], Node *node, unsigned long nnodes);

Node *ReadFile(const char file_dir[], unsigned long *nnodes,
               unsigned long *nways, unsigned long *nedges);
