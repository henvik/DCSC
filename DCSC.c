// Divide and conquer strong components. Algortihm developed by Fleischer et al.
//Implementation by Henrik Vikøren - part of master's thesis at NTNU, in collaboration with SINTEF 

#include<stdio.h>
#include<math.h>
#include<mpi.h>
#include<stdlib.h>
#include <stdbool.h>
#include<time.h>

#include"graph_utils.h"
#include"functions_DCSC.h"
#include"DCSC.h"

int main(int argc, char* argv[]){
	printf("Test implementation of DCSC \n");
   
   
	srand ( time(NULL) );

	LinkedList *G=importGridLinked("Grids/grid1src100.txt");

//	printLinkedList(G);

	//printAdj_Graph(G);

//List to store all the SCCs. This has to be optimalized
	LinkedList **order=malloc(100*sizeof(LinkedList*));
	int orderInd[100];
	DCSC(G);


	return 0;
}

void DCSC(LinkedList* G){
	if(G->num_vert==0){	

//		printf("End of recursion\n");
		return;
	}

	Node *pivot=get_Node(G,(rand()%G->num_vert));
	if(!pivot){
		printf("get_Node error\n");
		exit(0);
	}
	printf("Pivot node is: %d\n",pivot->vert_num);
	
	Node_pointers *to_be_removed_G=new_Node_pointers();

	LinkedList *desc=new_LinkedList();
	find_descendants(pivot,desc, to_be_removed_G);
	//LinkedList *desc = findDescendants(get_Node(G,pivot));
	//printLinkedList(desc);


	LinkedList *pred=new_LinkedList();
	find_predecessors(pivot,pred,to_be_removed_G);
//	LinkedList *pred=findPredecessors(get_Node(G,pivot));
	//printLinkedListPredecessors(pred);
		
	Node_pointers *to_be_removed_desc= find_union(pred,desc);
	Node_pointers *to_be_removed_pred=find_union(desc,pred);

// Remove the nodes of the SCC completely, at the same time as creating a new SCC to return. 
	//printNode_pointers(to_be_removed_G);

//	printf("Romving from pred\n");
	LinkedList *SCC=remove_from_graph(pred,to_be_removed_pred);

//	printf("Removing from desc\n");
	remove_from_graph(desc,to_be_removed_desc);
	
	//printf("Removing from G\n");
	remove_from_graph(G,to_be_removed_G);
	
	printf("SCC\n");
	printLinkedList(SCC);
//	printf("Desc\n");
//	printLinkedList(desc);
//	printf("Pred\n");
//	printLinkedList(pred);
//	printf("Rem\n");
//	printLinkedList(G);
	
//	printf("RECURSING\n");
	DCSC(pred);
	//printf("calling pred\n");
	DCSC(desc);
	DCSC(G);
	/*
	removeFromAdj_Graph(pred,SCC);
	removeFromAdj_Graph(desc,SCC);
	removeFromAdj_Graph(G,SCC);
	DCSC(pred);
	DCSC(desc);
	DCSC(G);
	*/
}


