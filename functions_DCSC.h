#ifndef FUNCTIONS_DCSC
#define FUNCTIONS_DCSC

typedef struct StackNode{
	Node *pointer; //points to th node
	struct StackNode *next;
}StackNode; 

typedef struct VisitStack{
	StackNode *top;
}VisitStack;

//STACKSTUFF



VisitStack* new_VisitStack();
StackNode* new_StackNode(Node* pointer);
void push_VisitStack(VisitStack *s,StackNode *n);
Node* pop_VisitStack(VisitStack *s);

Node_pointers* new_Node_pointers();
void add_pointer(Node_pointers *pointers, Node *node);

Node* find_descendants(Node *node, LinkedList *desc, Node_pointers *store);
Node* find_predecessors(Node *node, LinkedList *pred, Node_pointers *store);

void  FindDescendants(Node *pivot, LinkedList *desc, VisitStack *stack);
void FindPredecessors(Node *pivot, LinkedList *pred, VisitStack *stack);

Node_pointers* find_union(LinkedList* pred, LinkedList *desc);

#endif
