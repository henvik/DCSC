// Divide and conquer strong components. Algortihm developed by Fleischer et al.
//Implementation by Henrik Vikøren - part of master's thesis at NTNU, in collaboration with SINTEF 

#include<stdio.h>
#include<math.h>
#include<mpi.h>
#include<stdlib.h>
#include <stdbool.h>
#include<time.h>
#include <omp.h>

#include"graph_utils.h"
#include"functions_DCSC.h"
#include"DCSC.h"



int main(int argc, char* argv[]){
	int n_threads = omp_get_num_threads();
	printf("Test implementation of DCSC with %d threads. \n",n_threads);
	//seeding the random number generator, used for picking a pivot vertex.
	srand ( time(NULL) );

	//Imports the graph from a CSV file.
	int *ia,*ja,nv,ne;
	importGrid("Grids/graphRand100x100.txt",&ia,&ja,&nv,&ne);
	//printf("test %d \n",ia[rand()%nv]);
	LinkedList *G=convertGrid(ia,nv,ja);
//	LinkedList *G=importGridLinked("Grids/graphRand1000x1000.txt");
	printf("Size of graph is %d. \n",G->num_vert);
	//	printLinkedList(G);

	//A cutoff specifying for what size of the graph we will no longer do recursions in parallel.
	int cutoff=100; //has to be tuned
	
	//timing
	clock_t begin, end;
	double time_spent;

	begin = clock();
	LinkedList *order;
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			order =DCSC_parallel(G,cutoff);
		}
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("DCSC took: %f \n",time_spent);
	//printLinkedListSequence(order);

	return 0;
}




LinkedList* DCSC_parallel(LinkedList* G, int cutoff){
/*If DCSC is called on an empty graph it returns immediately*/
	if(G->num_vert<=cutoff){	
		return DCSC_serial(G);
	}

/*Finding pivot node. */
	int random=rand()%G->num_vert;
	Node *pivot=get_pivot(G,random);
	if(!pivot){
		printf("get_Node error. Size: %d and random %d\n",G->num_vert,random);
		exit(0);
	}

/*Finding descendants and predecessors of the pivot node. Save the nodes that are to be removed 
in to_be_removed_G. 
*/ 
//	Node_pointers *to_be_removed_G=new_Node_pointers();
	VisitStack *stack=new_VisitStack();
	LinkedList *desc=new_LinkedList();
	LinkedList *SCC=new_LinkedList();
	//find_descendants(pivot,desc, to_be_removed_G);
	FindDescendants(pivot,desc,stack);
	LinkedList *pred=new_LinkedList();
//	find_predecessors(pivot,pred,to_be_removed_G);
	FindPredecessors(pivot,pred,SCC,stack);
/* Finding the union of desc and pred. saving nodes that have to be removed from the pred and desc sets.*/		
//	Node_pointers *to_be_removed_desc= find_union(pred,desc);
//	Node_pointers *to_be_removed_pred=find_union(desc,pred);



/* Remove the nodes of the SCC completely, at the same time as creating a new SCC to return. */
	//printNode_pointers(to_be_removed_G);

//	LinkedList *SCC=remove_from_graph(pred,to_be_removed_pred);
//	remove_from_graph(desc,to_be_removed_desc);
//	remove_from_graph(G,to_be_removed_G);
	
	//printf("SCC\n");
//	printLinkedList(SCC);
	removeMarked(G,desc,pred);
	LinkedList *listOfLists[4];
	
//	Recursion


LinkedList *predReturn, *remReturn, *descReturn;
        #pragma omp task shared(predReturn) 
		predReturn =DCSC_parallel(pred,cutoff);
        #pragma omp task shared(remReturn)
			remReturn =	DCSC_parallel(G,cutoff);
	#pragma omp task shared(descReturn) 
			descReturn=	DCSC_parallel(desc,cutoff);
	#pragma omp taskwait
	listOfLists[0]=predReturn;	
	listOfLists[1]=	remReturn;
	listOfLists[2]=SCC;
	listOfLists[3]=descReturn;
	return mergeLinkedLists(listOfLists,4);


}

LinkedList* DCSC_serial(LinkedList* G){
/*If DCSC is called on an empty graph it returns immediately*/
	if(G->num_vert==0){	
		return G;
	}

/*Finding pivot node. */
	int random=rand()%G->num_vert;
	Node *pivot=get_pivot(G,random);
	if(!pivot){
		printf("get_Node error. Size: %d and random %d\n",G->num_vert,random);
		exit(0);
	}

/*Finding descendants and predecessors of the pivot node. Save the nodes that are to be removed 
in to_be_removed_G. 
*/ 
	//Node_pointers *to_be_removed_G=new_Node_pointers();
	LinkedList *SCC=new_LinkedList();	
	LinkedList *desc=new_LinkedList();
	LinkedList *pred=new_LinkedList();
	VisitStack *stack=new_VisitStack();
//	find_descendants(pivot,desc, to_be_removed_G);
	FindDescendants(pivot,desc,stack);
	FindPredecessors(pivot,pred,SCC,stack);
//	find_predecessors(pivot,pred,to_be_removed_G);

/* Finding the union of desc and pred. saving nodes that have to be removed from the pred and desc sets.*/		
	//Node_pointers *to_be_removed_desc= find_union(pred,desc);
	//Node_pointers *to_be_removed_pred=find_union(desc,pred);



/* Remove the nodes of the SCC completely, at the same time as creating a new SCC to return. */
	//printNode_pointers(to_be_removed_G);

//	LinkedList *SCC=remove_from_graph(pred,to_be_removed_pred);
//	remove_from_graph(desc,to_be_removed_desc);
//	remove_from_graph(G,to_be_removed_G);
	
//	printf("SCC\n");
//	printLinkedList(SCC);

	removeMarked(G,desc,pred);
	
	LinkedList *listOfLists[4];
	
//	Recursion

	listOfLists[0]=DCSC_serial(pred);
	listOfLists[1]=	DCSC_serial(G);
	listOfLists[2]=SCC;
	listOfLists[3]=DCSC_serial(desc);
	return mergeLinkedLists(listOfLists,4);


}


