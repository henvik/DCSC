#include<math.h>
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>
#include<stdbool.h>

#include"graph_utils.h"
#include"functions_DCSC.h"
#include"DCSC.h"


Node* find_descendants(Node *node, LinkedList *desc, Node_pointers *store){
	Node* new=new_Node(node->vert_num);
	add_node(desc,new);
	add_pointer(store,node);
	arc_t *current=node->children;
	Node *child;
	while(current!=NULL){
		if(isIn(desc,current->head)){
			child=get_Node(desc,current->head->vert_num);
			if(!child){
				printf("ERORR: NODE NOT FOUND\n");
				exit(0);
			}
			add_edge(new,child);
			current=current->next;
			continue;
		}
		child=find_descendants(current->head,desc,store);
		add_edge(new,child);
		current=current->next;
	}
	return new;
}

Node* find_predecessors(Node *node, LinkedList *pred, Node_pointers *store){
	Node* new=new_Node(node->vert_num);
	add_node(pred,new);
	add_pointer(store,node);
	arc_t *current=node->parents;
	Node *parent;
	while(current!=NULL){
		if(isIn(pred,current->head)){
			parent=get_Node(pred,current->head->vert_num);
			if(!parent){
				printf("ERORR: NODE NOT FOUND\n");
				exit(0);
			}
			add_edge(parent,new);
			current=current->next;
			continue;
		}
		parent=find_predecessors(current->head,pred,store);
		add_edge(parent,new);
		current=current->next;
	}
	return new;
}

LinkedList* findDescendants( Node *pivot){
	
	Node* pivotCopy =copy_Node(pivot);
	remove_backwards_edges(pivot);
	LinkedList* desc=new_LinkedList(0);
	add_node(desc,pivotCopy);
	visitChildren(desc,pivotCopy);
	return desc;
}

void visitChildren( LinkedList *desc, Node *node){
	arc_t* current=node->children;
	
	while(current!=NULL){
		if(isIn(desc,current->head)){
			current=current->next;
			continue;
		}
		add_node(desc,current->head);
		visitChildren(desc,current->head);
		current=current->next;
	}
	
	// for(int i=0;i<node->num_children;i++){
// 		if(isIn(desc,node->children[i])){
// 			continue;
// 		}
// 		add_node(desc,node->children[i]);
// 		visitChildren(desc,node->children[i]);
// 	}
// 	
}
 
LinkedList* findPredecessors( Node *pivot){

	Node* pivotCopy =copy_Node(pivot);
	remove_forward_edges(pivot);
	LinkedList* pred=new_LinkedList(0);
	add_node(pred,pivotCopy);
	visitParents(pred,pivotCopy);
	return pred;

}

void visitParents( LinkedList *pred, Node *node){
	arc_t* current=node->parents;
	while(current!=NULL){
		if(isIn(pred,current->head)){
			current=current->next;
			continue;
		}
		add_node(pred,current->head);
		visitParents(pred,current->head);
		current=current->next;
	}
	
// 	for(int i=0;i<node->num_parents;i++){
// 		if(isIn(pred,node->parents[i])){
// 			continue;
// 		}
// 		add_node(pred,node->parents[i]);
// 		visitParents(pred,node->parents[i]);
// 	}
	
}
Node_pointers* new_Node_pointers(){
	Node_pointers *new =malloc(sizeof(Node_pointers));
	new->list=malloc(sizeof(Node*)*4);
	new->size=0;
	new->capasity=4;
	return new;
}
bool inNode_pointers(Node* node, Node_pointers *pointers){
	for(int i=0;i<(pointers->size);i++){
		if(pointers->list[i]->vert_num==node->vert_num){
			return true;
		}
	}
	return false;
}


void add_pointer(Node_pointers *pointers, Node *node){
	
	if(!inNode_pointers(node,pointers)){
		if(pointers->size==pointers->capasity){
			pointers->list=realloc(pointers->list,sizeof(Node*)*pointers->capasity*2);
			pointers->capasity*=2;
		}
		pointers->list[pointers->size++]=node;
	}
}
Node_pointers* find_union(LinkedList* pred, LinkedList *desc){
	Node_pointers *SCC=new_Node_pointers();
	Node *current=desc->first;
	while(current!=NULL){
		if(isIn(pred,current)){
			add_pointer(SCC,current);
//			printf("Added %d\n",current->vert_num);
			current=current->next;
			continue;
		}
		current=current->next;
		
	}
	return SCC;
}

