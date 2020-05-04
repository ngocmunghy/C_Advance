#include<stdio.h>
#include<stdlib.h>
#include"jrb.h"

typedef JRB Graph;

Graph createGraph();
void addEdge(Graph g, int v1, int v2);
int adjacent(Graph g, int v1, int v2);
int getAdjacentVertices(Graph g, int v1, int *output);
void dropGraph(Graph g);

int main() {
	int i,n,output[100];
	Graph g = createGraph();
	addEdge(g,0,1);
	addEdge(g,0,2);
	addEdge(g,1,2);
	addEdge(g,1,3);
	n = getAdjacentVertices(g,1,output);
	if(n == 0)
		printf("Ko co dinh nao ke voi node 1\n");
	else
	{
		printf("Nhung dinh ke voi node 1: ");
		for(i=0;i<n;i++) 
			printf("%d ",output[i]);
		printf("\n");
	}
		dropGraph(g);
	return 0;
}

Graph createGraph() {
	return make_jrb();
}

int adjacent(Graph g, int v1, int v2) {
	JRB node, tree;
	node = jrb_find_int(g,v1);
	if(node == NULL) {
		return 0;
	}
	else {
		tree = (JRB)(jval_v(node->val));
		node = jrb_find_int(tree,v2);
		if (node == NULL)
			return 0;
		else return 1;
	}
}

void addEdge(Graph g, int v1, int v2) {
	JRB node,tree;
	if(!adjacent(g,v1,v2)) {
		node = jrb_find_int(g,v1);
		if(node == NULL) {
			tree = createGraph();
			jrb_insert_int(g,v1,new_jval_v(tree));
			jrb_insert_int(tree, v2, new_jval_i(1));
		} else {
			tree = (JRB)jval_v(node->val);
			jrb_insert_int(tree,v2,new_jval_i(1));
		}
	}
	if(!adjacent(g,v2,v1)) {
		node = jrb_find_int(g,v2);
		if(node == NULL) {
			tree = createGraph();
			jrb_insert_int(g,v2,new_jval_v(tree));
			jrb_insert_int(tree,v1,new_jval_i(1));
		}
		else {
			tree = (JRB)(jval_v(node->val));
			jrb_insert_int(tree,v1,new_jval_i(1));
		}
	}
}

int getAdjacentVertices(Graph g, int v1, int *output) {
	JRB node = jrb_find_int(g,v1);
	int total = 0;
	if(node == NULL) 
		return 0;
	JRB tree = (JRB)jval_v(node->val);
	jrb_traverse(node, tree) {
		output[total++] = jval_i(node->key); 
	}
	return total;
} 

void dropGraph(Graph graph) {
	JRB node,tree;
	jrb_traverse(node,graph)
	{
		tree = (JRB)jval_v(node->val);
		jrb_free_tree(tree);
	}
	jrb_free_tree(graph);
	return;
}