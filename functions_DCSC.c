#include<math.h>
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>
#include<stdbool.h>

#include"graph_utils.h"
#include"functions_DCSC.h"
#include"DCSC.h"


VisitStack* new_VisitStack(){
	VisitStack *new = malloc(sizeof(VisitStack));
	new->top=NULL;
	return new;
}	

StackNode* new_StackNode(Node* pointer){
	StackNode *new=malloc(sizeof(StackNode));
	new->pointer=pointer;
	new->next=NULL;
}

void push_VisitStack(VisitStack *s,StackNode *n){
	n->next=s->top;
	s->top=n;
}
Node* pop_VisitStack(VisitStack *s){
	if(s->top==NULL){
		return NULL;
	}
	Node *tmp=s->top->pointer;
	StackNode* del = s->top;
	s->top=s->top->next;
	free(del);
	return tmp;
}

void clearStatuses(LinkedList *G){
	Node *current=G->first;
	while(current){
		current->desc_status=NULL;
		current->pred_status=NULL;
		current=current->next;
	}
}

void  FindDescendants(Node *pivot, LinkedList *desc, VisitStack *stack){
	
	arc_t *current;
	pivot->desc_status=new_Node(pivot->vert_num);
	add_node(desc,pivot->desc_status);
	while(pivot){

		current=pivot->children;
		while(current){
			if(current->head->desc_status){
				add_edge(pivot->desc_status,current->head->desc_status);
				current=current->next;
				continue;
			}
			current->head->desc_status=new_Node(current->head->vert_num);
			add_node(desc,current->head->desc_status);
			add_edge(pivot->desc_status,current->head->desc_status);
			push_VisitStack(stack,new_StackNode(current->head));
			current=current->next;
		}
		pivot=pop_VisitStack(stack);		
	}

} 

void FindPredecessors(Node *pivot, LinkedList *pred, VisitStack *stack){
	
	arc_t *current;
	pivot->pred_status=new_Node(pivot->vert_num);
	add_node(pred,pivot->pred_status);
	while(pivot){
		current=pivot->parents;
		while(current){
			if(current->head->pred_status){
				add_edge(current->head->pred_status,pivot->pred_status);
				current=current->next;
				continue;
			}
			current->head->pred_status=new_Node(current->head->vert_num);
			add_node(pred,current->head->pred_status);
			add_edge(current->head->pred_status,pivot->pred_status);
			push_VisitStack(stack,new_StackNode(current->head));
			current=current->next;
		}
		pivot=pop_VisitStack(stack);
	}
}


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

