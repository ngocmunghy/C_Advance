#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#define MAX 100

typedef JRB Graph;

Graph createGraph();
void addEdge(Graph g, char *v1, char *v2);
int adjacent(Graph g, char *v1, char *v2);
int getAdjacentVertices(Graph g, char *v1, char output[][MAX]);
void dropGraph(Graph g);

int main() {
	int n, i;
	char output[20][MAX]; 
	Graph g = createGraph();
	addEdge(g,"V0","V1");
	addEdge(g,"V0","V2");
	addEdge(g,"V1","V2");
	addEdge(g,"V1","V3");
	addEdge(g,"V3","V0");
	n = getAdjacentVertices(g,"V0",output);
	if(n == 0)
		printf("Ko co dinh nao ke voi node V0.\n");
	else
	{
		printf("Nhung dinh ke voi node V0: ");
		for(i=0;i<n;i++) 
			printf("%s ",output[i]);
		printf("\n");
	}
	dropGraph(g);
	return 0;
}

Graph createGraph() {
	return make_jrb();
}

int adjacent(Graph g, char *v1, char *v2) {
	JRB node, tree;
	node = jrb_find_str(g,v1);
	if(node == NULL) {
		return 0;
	}
	else {
		tree = (JRB)(jval_v(node->val));
		node = jrb_find_str(tree,v2);
		if (node == NULL)
			return 0;
		else return 1;
	}
}

void addEdge(Graph g, char *v1, char *v2) {
	JRB node,tree;
	if(!adjacent(g,v1,v2)) {
		node = jrb_find_str(g,v1);
		if(node == NULL) {
			tree = createGraph();
			jrb_insert_str(g,strdup(v1),new_jval_v(tree));
			jrb_insert_str(tree, strdup(v2), new_jval_i(1));
		} else {
			tree = (JRB)jval_v(node->val);
			jrb_insert_str(tree,strdup(v2),new_jval_i(1));
		}
	}
	if(!adjacent(g,v2,v1)) {
		node = jrb_find_str(g,v2);
		if(node == NULL) {
			tree = createGraph();
			jrb_insert_str(g,strdup(v2),new_jval_v(tree));
			jrb_insert_str(tree,strdup(v1),new_jval_i(1));
		}
		else {
			tree = (JRB)(jval_v(node->val));
			jrb_insert_str(tree,strdup(v1),new_jval_i(1));
		}
	}
}

int getAdjacentVertices(Graph g, char *v1, char output[][MAX]) {
	JRB node = jrb_find_str(g,v1);
	int total = 0;
	if(node == NULL) 
		return 0;
	JRB tree = (JRB)jval_v(node->val);
	jrb_traverse(node, tree) {
		//output[total++] = jval_s(node->key); 
		strcpy(output[total++],jval_s(node->key));
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