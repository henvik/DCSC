#include<stdio.h>
#include<math.h>
#include<mpi.h>
#include <stdbool.h>
#include<stdlib.h>

#include"graph_utils.h"
#include"functions_DCSC.h"
#include"DCSC.h"


#define INITIALIZING_SIZE 2

//------------------------------------LINKED LISTS --------------------------------------------


LinkedList* new_LinkedList(){
	LinkedList *new=malloc(sizeof(LinkedList));
	new->num_vert=0;
	new->first=NULL;
	new->last=NULL;
	return new;
}

Node* new_Node(int vert_num){
	Node* new=(Node*)malloc(sizeof(Node));
	new->vert_num=vert_num;

	new->parents=NULL;
	new->children=NULL;
	
	new->next=NULL;
	new->prev=NULL;
	return new;
}





//adds an edge from source to terminal
void add_edge(Node *source, Node *terminal){
	arc_t *forward=malloc(sizeof(arc_t));
	forward->head=terminal;
	forward->next=NULL;

	arc_t *current=source->children;
	if(current==NULL){
		source->children=forward;
	}else{
		while(current->next!=NULL){	
			current=current->next;
		}
		current->next=forward;
	}
	arc_t *backwards=malloc(sizeof(arc_t));
	backwards->head=source;
	backwards->next=NULL;

	current=terminal->parents;
	if(current==NULL){
		terminal->parents=backwards;
	}else{
		while(current->next!=NULL){
			current=current->next;
		}
		current->next=backwards;
	}
	// if(source->num_children==0){
// 		source->num_children=1;
// 		source->children=(Node**)malloc(sizeof(Node*));
// 		source->children[0]=terminal;
// 	}else{
// 		source->num_children++;
// 		source->children=realloc(source->children,sizeof(Node*)*source->num_children);
// 		source->children[source->num_children-1]=terminal;
// 	}
// 	if(terminal->num_parents==0){
// 		terminal->num_parents=1;
// 		terminal->parents=(Node**)malloc(sizeof(Node*));
// 		terminal->parents[0]=source;
// 	}else{
// 		terminal->num_parents++;
// 		terminal->parents=realloc(terminal->parents,sizeof(Node*)*terminal->num_parents);
// 		terminal->parents[terminal->num_parents-1]=source;
// 	}
}

//add an node to the graph
void add_node(LinkedList *graph,Node *node){
	

	if(graph->num_vert==0){
		graph->num_vert++;
		graph->first=node;
		graph->last=node;
		node->next=NULL;
		node->prev=NULL;
	}
	else{
		graph->num_vert++;
		graph->last->next=node;
		node->prev=graph->last;
		graph->last=node;
		node->next=NULL;
	}
	
// // 	if(graph->capasity==0){
// // 		graph->capasity=INITIALIZING_SIZE;
// // 		graph->nodes=(Node**)malloc(sizeof(Node*)*graph->capasity);
// // 	}
// // 	if(graph->capasity==graph->num_vert){
// // 		graph->capasity*=2;
// // 		graph->nodes=realloc(graph->nodes,graph->capasity*sizeof(Node*));
// // 	}
// // 	graph->nodes[graph->num_vert]=node;
// // 	node->list_index=graph->num_vert;
// // 	graph->num_vert++;
}

Node* get_Node(LinkedList *graph,int vert_num){
	if(graph->first==NULL){
		printf("Graph is empty\n");
		return NULL;
	}
	Node *desired=graph->first;
	while(desired->vert_num!=vert_num && desired->next!=NULL){
		desired=desired->next;
	}
	if(desired==NULL){
		printf("The desired node not found\n");
	}
	return desired;
}

arc_t* new_arc(Node *head, arc_t *next){
	arc_t *new=malloc(sizeof(arc_t));
	new->head=head;
	new->next=next;
	return new;
}


Node* copy_Node(Node *node){
	Node* new=new_Node(node->vert_num);
	if(node->children!=NULL){	
		new->children=new_arc(node->children->head,node->children->next);
		arc_t *new_current=new->children;
		arc_t *current=node->children->next;
		while(current!=NULL){
			new_current->next=new_arc(current->head,current->next);
			new_current=new_current->next;
			current=current->next;
		}	
	}
	if(node->parents!=NULL){
		new->parents=new_arc(node->parents->head,node->parents->next);
		arc_t *new_current=new->parents;
		arc_t *current=node->parents->next;
		while(current!=NULL){
			new_current->next=new_arc(current->head,current->next);
			new_current=new_current->next;
			current=current->next;
		}
	}
	new->next=node->next;
	new->prev=node->prev;

	return new;
}


void free_node(Node *node){
	if(node->children!=NULL){
		arc_t* to_be_del=node->children;
		arc_t* current=node->children->next;
		while(current!=NULL){
			free(to_be_del);
			to_be_del=current;
			current=current->next;
		}
	}
	if(node->parents!=NULL){
		arc_t* to_be_del=node->parents;
		arc_t* current=node->parents->next;
		while(current!=NULL){
			free(to_be_del);
			to_be_del=current;
			current=current->next;
		}
	}
	free(node->parents);
	free(node->children);
	free(node);
}

// void free_graph(LinkedList *graph){
// 	for(int i=0; i<graph->num_vert;i++){
// 		free_node(graph->nodes[i]);
// 	}
// 	free(graph->nodes);
// }


//Read a .csv file an make a directed graph in sparse matrix format
/*The file has to have the following format:
- The first line consists of a single number, indication the number of vertices in the graph.
- line number i indicates contains a list of vertices with edges from the i'th vertice on the format: k,l,m; 
*/
LinkedList* importGridLinked(char* file){
  
    FILE * fp;
    char line[100];
	int len=0;
	int i,l,charLeft,tmp;	
	
	char* buffer = (char*)malloc(sizeof(char)*32); //Char buffer
	fp=fopen(file,"r");	
	if(fp==NULL){
		printf("File failed to open\n");
	   exit(EXIT_FAILURE);
	}

	int line_nr=0;
	//Reads the first line
	fgets(line,60,fp);
	int numOfLines=atoi(line);
	LinkedList *graph=new_LinkedList();
	add_node(graph,new_Node(0));
	for(int i=1; i<numOfLines;i++){
		 add_node(graph,new_Node(i));
	}
	
	//reads the file line by line, starting at the second line
	while(fgets(line,60,fp)){
	     i=0;
	     l=0;
	     charLeft=1;
	   //  As long as there is characters in the line
	     while(charLeft){
	     	//If we find the delimiter signaling the end of an integer
		   if(line[i]==','){ 
		   	  //We mark the end of the current integer and convert it to an int and add it to ja
		   	  buffer[l++]='\0';
			  l=0;
		      tmp=atoi(buffer);
		      add_edge(get_Node(graph,line_nr),get_Node(graph,tmp));
		      i++;
		  //If we encounter the end of a line
	       }else if(line[i]==';'){
	       	  	charLeft=0;
	       	  	line_nr++;
	       }
	       //If non of the above we just read the next char in the line
	    	else{
	   		  buffer[l++]=line[i++];
	   	  }
	   }
	}
	fclose(fp);
	return graph;
}


void printLinkedList(LinkedList *graph){
	if(graph->num_vert==0){
//		printf("Empty list \n");
		return;
	}
	//printf("First is: %d, last is: %d\n",graph->first->vert_num,graph->last->vert_num);
	Node* node=graph->first;
	while(node!=NULL){
		printNode(node);
		node=node->next;
	}
}

void printNode(Node *node){
	if(node==NULL){
		printf("Node is null pointer\n");
	}else{
		printf("Node %d has edges: ",node->vert_num);
		arc_t* current=node->children;
		while(current!=NULL){
			printf("%d, ",current->head->vert_num);
			current=current->next;
		
		}
		// for(int i=0;i<node->num_children;i++){
// 			printf("%d, ", (node->children[i])->vert_num);
// 		}
	}
	printf("\n");
}

void printLinkedListPredecessors(LinkedList *graph){
	if(graph->num_vert==0){
		printf("Empty list \n");
		return;
	}
	Node* node=graph->last;
	printNodeParents(node);
	while(node->prev!=NULL){
		node=node->prev;
		printNodeParents(node);
	}
}

void printNodeParents(Node *node){
	if(node==NULL){
		printf("Node is null pointer\n");
	}else{
		printf("Node %d has parents: ",node->vert_num);
		arc_t* current=node->parents;
		while(current!=NULL){
			printf("%d, ",current->head->vert_num);
			current=current->next;
		
		}
		// for(int i=0;i<node->num_parents;i++){
// 			printf("%d, ", (node->parents[i])->vert_num);
// 		}
	}
	printf("\n");
}



//checks if node is contained in the G. O(n)
bool isIn(LinkedList *G, Node *node){
	Node *current=NULL;
	current=G->first;
	while(current!=NULL){
		if(current->vert_num==node->vert_num){
			return true;
		}
		current=current->next;
	}
	return false;

	// for(int i=0; i<G->num_vert;i++){
// 		if(G->nodes[i]->vert_num==node->vert_num){
// 			return true;
// 		}
// 	}
// 	return false;
}

void remove_node(LinkedList *G,Node* node){
	if(node==NULL){
		printf("Node is NULL pointer\n");
		return;
	}
	if(!G->num_vert){
		printf("No nodes to remove\n");
		return;
	}
//	printf("To be removed: %d. %d nodes in G\n",node->vert_num,G->num_vert);
	//remove edges
	remove_forward_edges(node);
	remove_backwards_edges(node);
	//remove node
	if(G->first->vert_num==node->vert_num){
		//printf("First\n");
		if(node->next!=NULL){
			node->next->prev=NULL;
		}
		G->first=node->next;
	}else if(G->last->vert_num==node->vert_num){
		//printf("Last\n");
		G->last=node->prev;
		node->prev->next=NULL;
	}else{
		//printf("Third option\n");
	//	if(node->next==NULL){
		//	printf("next is NULL. Node is: %d, the last is %d\n",node->vert_num,G->last->vert_num);
		//	printLinkedList(G);
	//	}
		node->next->prev=node->prev;
	//	if(node->prev==NULL){
	//		printf("prev is NULL\n");
	//	}
		node->prev->next=node->next;
	}
	//printf("Removed %d\n",node->vert_num);
	G->num_vert--;
	
}




void remove_forward_edges(Node* node){
	
	arc_t *current=node->children;
	arc_t *to_be_del=current;
	while(to_be_del!=NULL){
		current=current->next;
		remove_edge(node,to_be_del->head);
		to_be_del=current;
	}
	node->children=NULL;

}

void remove_backwards_edges(Node* node){
	
	arc_t *current=node->parents;
	arc_t *to_be_del=current;
	while(current!=NULL){
		current=current->next;
		remove_edge(to_be_del->head,node);
		to_be_del=current;
	}
	node->parents=NULL;

}


void remove_edge(Node* source, Node* terminal){
	arc_t *current=source->children;
	arc_t *prev=source->children;
	arc_t *to_be_del;
	while(current!=NULL){
		if(current->head==terminal){
			to_be_del=current;
			if(current==source->children){
				source->children=current->next;	
			}else{
				prev->next=current->next;
			}

			free(to_be_del);
			break;
		}else{
			prev=current;
			current=current->next;
		}
	} 
	
	current=terminal->parents;
	prev=current;
	
	while(current!=NULL){
		if(current->head==source){
			to_be_del=current;
			if(current==terminal->parents){
				terminal->parents=current->next;	
			}else{
				prev->next=current->next;		
			}

			free(to_be_del);
			break;
		}
		prev=current;
		current=current->next;
	} 

}

//Removes the sub_graph from graph
LinkedList* remove_from_graph(LinkedList* graph, Node_pointers* sub_graph){
	//printf("Will remove from: \n");
	//printLinkedList(graph);
	LinkedList *SCC=new_LinkedList();
	for(int i=0;i<sub_graph->size;i++){
		add_node(SCC,copy_Node(sub_graph->list[i]));
		remove_node(graph,sub_graph->list[i]);
	}
	return SCC;
}

void printNode_pointers(Node_pointers* pointers){
	printf("Start\n");
	for(int i=0;i<pointers->size;i++){
		printf("%d \n ",pointers->list[i]->vert_num);
	}
	printf("Done\n");
}

//--------------------------ADJECACY LISTS -------------------------------------------


void printAdj_Graph(Adj_Graph G){
	if(G.mapped){
		printMappedGraph(G.ia,G.ja,G.map,G.size_ia);
	}else{
		printGraph(G.ia, G.ja, G.size_ia);
	}
	
	//NEED TO WRITE METHOD FOR PRINTING SCCs	
}
void printGraph(int* ia, int *ja, int nv){
	int i,j;
	for(i=0;i<nv;i++){
		printf("Vertex %d has edges to: ",i);
		for(j=ia[i];j<ia[i+1];j++){
			printf("%d, ",ja[j]);
		}
		printf("\n");
	}
}

void printMappedGraph(int* ia, int *ja,int *map, int nv){
	int i,j;
	for(i=0;i<nv;i++){
		printf("Vertex %d has edges to: ",map[i]);
		for(j=ia[i];j<ia[i+1];j++){
			printf("%d, ",ja[j]);
		}
		printf("\n");
	}
}


Adj_Graph importGraph(char* file){
	Adj_Graph G;
	G.mapped=false;
	importGrid(file,&G.ia, &G.ja, &G.size_ia, &G.size_ja);
	printf("G.size_ia: %d, G.size_ja: %d \n",G.size_ia,G.size_ja);
	return G;
}

//Read a .csv file an make a directed graph represented as an adjecancy list
/*The file has to have the following format:
- The first line consists of a single number, indication the number of vertices in the graph.
- line number i indicates contains a list of vertices with edges from the i'th vertice on the format: k,l,m; 
*/
void importGrid(char* file, int **ia, int **ja, int *nv, int *ne){
    
    FILE * fp;
    char line[100];
	int len=0;
	
	int l,i,tmp,charLeft;  
    int jaINDEX=0; //Index counter for the array ja.
    int numOfLines; //to store the number of lines

	char* buffer = (char*)malloc(sizeof(char)*32); //Char buffer
	fp=fopen(file,"r");	
	if(fp==NULL){
		printf("File failed to open\n");
	   exit(EXIT_FAILURE);
	}
	int line_nr=0;
	//Reads the first line
	fgets(line,60,fp);
	numOfLines=atoi(line);
	*nv=numOfLines;
// Allocating memory
	int *Pia=(int *)malloc((numOfLines+1)*sizeof(int));
	int *Pja=(int *)malloc(numOfLines*4*sizeof(int));	
	//reads the file line by line, starting at the second line
	while(fgets(line,60,fp)){
	     i=0;
	     l=0;
	     charLeft=1;
	     int edgeCount=0;
	   //  As long as there is characters in the line
	     while(charLeft){
	     	//If we find the delimiter signaling the end of an integer
		   if(line[i]==','){
		   		//If this is the first edge from the current vertex
		   	  if(edgeCount==0){
		   	  	edgeCount++;
		   	    Pia[line_nr++]=jaINDEX; 
		   	  }
		   	  //We mark the end of the current integer and convert it to an int and add it to ja
		   	  buffer[l++]='\0';
			  l=0;
		      tmp=atoi(buffer);
		      Pja[jaINDEX++]=tmp;
		      i++;
		  //If we encounter the end of a line
	       }else if(line[i]==';'){
	       	  //If the line was empty we have to mark this
	       	  if(i==0){
	       	  	charLeft=0;
	       	  	Pia[line_nr++]=jaINDEX; 
	       	  	break;
	       	  }
	         charLeft=0;
	       }
	       //If non of the above we just read the next char in the line
	        else{
	   		  buffer[l++]=line[i++];
	   	  }
	   }
	}
	if(Pia[line_nr-1]==jaINDEX){
			Pia[line_nr]=jaINDEX;
	}
	else{
		Pia[line_nr]=jaINDEX;
	}
	//Set the pointers(couldn't do it any other way?)
	*ia=Pia;
	*ja=Pja;
	*ne=jaINDEX;
	fclose(fp);
}
