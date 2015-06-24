#ifndef GRAPH_UTILS_INCLUDED
#define GRAPH_UTILS_INCLUDED

#define PRED 1
#define DESC 2
#define BLANK 0
//-------------------------------Linked lists--------------------------

typedef struct arc_t{
	struct Node* head;		//Pointer to node which the arc points to
	struct arc_t* next;		//pointer to the next arc
}arc_t;


typedef struct Node{
	int vert_num;			//the vertex number in the global graph
	struct Node *next;			//the next node in the list
	struct Node *prev;			//the prev node in the list

	arc_t *parents;		//list of the direct predecessors
	arc_t *children;		//list of the direct descendants
	
	struct Node *desc_status;	//indicator of already discovered descendants
	struct Node *pred_status;	//indicator of already discovered predecessors
}Node;


typedef struct LinkedList{
	int num_vert;			//number of vertices in graph

	Node *first;			//pointer to the first node in the graph
	Node *last;				//pointer to the last node in the graph
}LinkedList;

typedef struct Node_pointers{
        struct Node** list;
        int size;
        int capasity;
}Node_pointers;


/* Creates a new instance of the structure LinkedList on the heap. Returns a pointer to it's location. */
LinkedList* new_LinkedList();

/* Creates a new instance of the structure Node on the heap. Returns a pointer to it's location.  */
Node* new_Node(int vert_num);

/*Adds a node to a graph*/
void add_node(LinkedList *graph,Node *node);

/* Adds an edge from the source to the terminal */
void add_edge(Node *source, Node *terminal);

/* Copies the contents of a node to a new instance */
Node* copy_Node(Node *node);

/* Frees a node, including all its edges */ 
void free_node(Node **node);

/* Frees a LinkedList, including all its nodes */
void free_LinkedList(LinkedList **G);

/* Imports a grid from a file and saves it as a LinkedList*/
LinkedList* importGridLinked(char* file);

/* Converts a grid from adjacency list representation to LinkedList */
LinkedList* convertGrid(int *ia,int size_ia , int *ja);

// imports grid from file, saves it as an adjecency list in ia and ja. nv is the number of vertices. ne is the number of edges
void importGrid(char* file, int **ia, int **ja, int *nv, int *ne);

//Printing
void printNode(Node *node);
void printLinkedList(LinkedList *graph);
void printNodeParents(Node *node);
void printLinkedListPredecessors(LinkedList *graph);
void printNode_pointers(Node_pointers* pointers);
void printLinkedListSequence(LinkedList *graph);

/* Checks if a certain node is a part of a graph */
bool isIn(LinkedList *G, Node *node);

/* Finds a specified node based on the vertex number */
Node* get_Node(LinkedList *graph,int vert_num);

/* Returns the num_in_graph node in the graph*/
Node* get_pivot(LinkedList *graph, int num_in_graph);


//Remove nodes and edges
/* Removes a node from a graph*/
void remove_node(LinkedList *G,Node* node);

/* removes an edge between two nodes */
void remove_edge(Node* source, Node* terminal);

/* Removes all the forward edges of a node*/
void remove_forward_edges(Node* node);

/*Removes all the backward edges of a node */
void remove_backwards_edges(Node* node);

/*  removes a subset of nodes from a graph */
LinkedList* remove_from_graph(LinkedList* graph, Node_pointers* sub_graph);

/* Removes nodes from the graph based on their statuses*/
void removeMarked(LinkedList *G, LinkedList *desc, LinkedList *pred, LinkedList *SCC);

//Appends the second list onto the first
void appendLinkedLists(LinkedList *first, LinkedList **second);

/*Merges n LinkedLists into one*/
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
