#ifndef FUNCTIONS_DCSC
#define FUNCTIONS_DCSC


Node_pointers* new_Node_pointers();
void add_pointer(Node_pointers *pointers, Node *node);

Node* find_descendants(Node *node, LinkedList *desc, Node_pointers *store);
Node* find_predecessors(Node *node, LinkedList *pred, Node_pointers *store);

LinkedList* findDescendants(Node *pivot);
void visitChildren(LinkedList *desc, Node *node);

LinkedList* findPredecessors(Node *pivot);
void visitParents(LinkedList *pred, Node *node);

Node_pointers* find_union(LinkedList* pred, LinkedList *desc);

#endif
