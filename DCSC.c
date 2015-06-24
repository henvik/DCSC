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
	int n_threads = atoi(argv[1]);
	omp_set_num_threads(n_threads);
	omp_set_nested(true);
	printf("Test implementation of DCSC with %d threads. \n",n_threads);
	//seeding the random number generator, used for picking a pivot vertex.
	srand ( time(NULL) );

	//Imports the graph from a CSV file.
	int *ia,*ja,nv,ne;
	importGrid("TestGrids/test16x16x16.txt",&ia,&ja,&nv,&ne);
	LinkedList *G=convertGrid(ia,nv,ja);
	free(ia);
	free(ja);
	printf("Size of graph is %d. \n",G->num_vert);

	//A cutoff specifying for what size of the graph we will no longer do recursions in parallel.
	int cutoff=G->num_vert/10; //has to be tuned
	printf("Cutoff is %d. \n",cutoff);
	//timing
	clock_t begin, end;
	double time_spent;

	begin = clock();
	LinkedList *order;
	#pragma omp parallel num_threads(n_threads)
	{
		#pragma omp single
		{
			order =DCSC_parallel(G,cutoff);
		}

	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("DCSC took: %f \n",time_spent);
	//printLinkedListSequence(order);
	
	free_LinkedList(&order);
	return 0;
}




LinkedList* DCSC_parallel(LinkedList* G, int cutoff){
/*If DCSC is called on an empty graph it returns immediately*/
	if(G->num_vert<=cutoff){	
		return DCSC_serial(G);
	}

/*Finding pivot node. */
	Node *pivot=get_pivot(G,rand()%G->num_vert);
	if(!pivot){
		printf("get_Node error.\n");
		exit(0);
	}

/*Finding descendants and predecessors of the pivot node. Save the nodes that are to be removed 
in to_be_removed_G. 
*/ 
	VisitStack *desc_stack=new_VisitStack();
	VisitStack *pred_stack=new_VisitStack();
	LinkedList *desc=new_LinkedList();
	LinkedList *SCC=new_LinkedList();
	LinkedList *pred=new_LinkedList();
	#pragma omp task shared(desc)
	{
		FindDescendants(pivot,desc,desc_stack);
	}
	#pragma omp task shared(pred)
	{
		FindPredecessors(pivot,pred,pred_stack);
	}
	#pragma omp taskwait
	free(desc_stack),free(pred_stack);
//	printLinkedList(SCC);
	removeMarked(G,desc,pred,SCC);
	LinkedList *listOfLists[4];
//	Recursion


LinkedList *predReturn, *remReturn, *descReturn;
        #pragma omp task shared(predReturn)
	{
		predReturn =DCSC_parallel(pred,cutoff);
	}
        #pragma omp task shared(remReturn)
	{
		remReturn =DCSC_parallel(G,cutoff);
	}
	#pragma omp task shared(descReturn)
	{
		descReturn=DCSC_parallel(desc,cutoff);
	}
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
	Node *pivot=get_pivot(G,rand()%G->num_vert);
	if(!pivot){
		printf("get_Node error\n");
		exit(0);
	}

/*Finding descendants and predecessors of the pivot node. Save the nodes that are to be removed 
in to_be_removed_G. 
*/ 
	LinkedList *SCC=new_LinkedList();	
	LinkedList *desc=new_LinkedList();
	LinkedList *pred=new_LinkedList();
	VisitStack *stack=new_VisitStack();

	FindDescendants(pivot,desc,stack);
	FindPredecessors(pivot,pred,stack);
	free(stack);

//	printLinkedList(SCC);

	removeMarked(G,desc,pred,SCC);
	
	LinkedList *listOfLists[4];
	
//	Recursion

	listOfLists[0]=DCSC_serial(pred);
	listOfLists[1]=	DCSC_serial(G);
	listOfLists[2]=SCC;
	listOfLists[3]=DCSC_serial(desc);
	return mergeLinkedLists(listOfLists,4);


}


