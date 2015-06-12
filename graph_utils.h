#ifndef GRAPH_UTILS_INCLUDED
#define GRAPH_UTILS_INCLUDED

#define PRED 1
#define DESC 2
#define BLANK 0
//-------------------------------Linked lists--------------------------

typedef struct arc_t{
	struct Node* head;
	struct arc_t* next;
}arc_t;


typedef struct Node{
	int vert_num;			//the vertex number in the global graph
	struct Node *next;			//the next node in the list
	struct Node *prev;			//the prev node in the list

	arc_t *parents;		//list of the direct predecessors
	arc_t *children;		//list of the direct descendants
	
	struct Node *desc_status;	//indicator of allready discoverd vertices
	struct Node *pred_status;
	struct Node *SCC_status;
}Node;


typedef struct LinkedList{
	int num_vert;			//number of vertices in graph

	Node *first;			//pointer to the first node in the graph
	Node *last;
}LinkedList;

typedef struct Node_pointers{
        struct Node** list;
        int size;
        int capasity;
}Node_pointers;


//Adding stuff to graphs
LinkedList* new_LinkedList();
Node* new_Node(int vert_num);
void add_node(LinkedList *graph,Node *node);
void add_edge(Node *source, Node *terminal);
Node* copy_Node(Node *node);

//Free 
void free_node(Node *node);
void free_graph(LinkedList *graph);

//Importing graph from text-file
LinkedList* importGridLinked(char* file);
LinkedList* convertGrid(int *ia,int size_ia , int *ja);

//Printing
void printNode(Node *node);
void printLinkedList(LinkedList *graph);
void printNodeParents(Node *node);
void printLinkedListPredecessors(LinkedList *graph);
void printNode_pointers(Node_pointers* pointers);
void printLinkedListSequence(LinkedList *graph);

//Read and check stuff
bool isIn(LinkedList *G, Node *node);
Node* get_Node(LinkedList *graph,int vert_num);
Node* get_pivot(LinkedList *graph, int num_in_graph);


//Remove nodes and edges
void remove_node(LinkedList *G,Node* node);
void remove_edge(Node* source, Node* terminal);
void remove_forward_edges(Node* node);
void remove_backwards_edges(Node* node);
LinkedList* remove_from_graph(LinkedList* graph, Node_pointers* sub_graph);
void removeMarked(LinkedList *G, LinkedList *desc, LinkedList *pred);

//Merge LinkedLists
void appendLinkedLists(LinkedList *first, LinkedList *second);
LinkedList* mergeLinkedLists(LinkedList **listOfLists, int n);

//-------------------------------Adjecancy lists-----------------------

//Struct for Graph, represented with adjacency list
typedef struct{
	int size_ia;
	int size_ja;
	int *ia;
	int *ja;
	//If the graph is distributed we use the map to store the global vertex-numbers
	bool mapped;
	int *map;
	//If the graph contains strongly connected components we store the indices
	bool contain_scc;
	int* scc_index;
	//Option for sorted graph
	bool sorted;
	int* order;
}Adj_Graph;


//prints a Graph, in the appropriate way
void printAdj_Graph(Adj_Graph G);
//prints a mapped graph
void printMappedGraph(int* ia, int *ja,int *map, int nv);
//prints an unmapped graph
void printGraph(int* ia, int *ja, int nv);

// imports grid from file, saves it as an adjecency list in ia and ja. nv is the number of vertices
void importGrid(char* file, int **ia, int **ja, int *nv, int *ne);

//imports graph. Returns a Adj_Graph struct containing all necessary information
Adj_Graph importGraph(char* file);
#endif
