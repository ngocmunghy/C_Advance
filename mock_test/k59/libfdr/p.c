#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define INFINITIVE_VALUE 9999

typedef struct {
	JRB edges;
	JRB vertices;
} Graph;

FILE *f;
int numOfV,numOfE;
int output[10],path[10],length;

Graph createGraph();
void dropGraph(Graph graph);
void addVertex(Graph graph, int id, char *name);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
char *getVertex(Graph graph, int id);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
int getNumOfV(Graph graph);
int getNumOfE(Graph graph);
void dropGraph(Graph graph);
void readFile_v(FILE *f, char* name);
void readFile_w(FILE *f, char* name);
void swap(int *a, int *b);

Graph graph;

int main(int argc, char* argv[]) {

	graph = createGraph();

	if(argc < 2) {
		printf("Qua it tham so duoc cung cap\n");
	}

		else if(strcmp(argv[1],"-h") == 0)
			printf("C-Advanced, HK20152\n");
	


		else if(strcmp(argv[1],"-v") == 0) {
			readFile_v(f,argv[2]);	

		}


		else if(strcmp(argv[1],"-n") == 0) {
			readFile_w(f,argv[2]);
			int id = atoi(argv[3]);
			int total = outdegree(graph,id,output);
			printf("%d\n",total);
			for(int i=0;i<total;i++) {
				printf("%d %s %d\n", output[i],getVertex(graph,output[i]),(int)getEdgeValue(graph,id,output[i]));
			}
		}



		else if(strcmp(argv[1],"-w") == 0) {
			readFile_w(f,argv[2]);
			int id1 = atoi(argv[3]);
			int id2 = atoi(argv[4]);
			int value = (int)getEdgeValue(graph,id1,id2);
			if(value == INFINITIVE_VALUE)
				value = -1;
			printf("Khoang cach giua %d va %d : %d\n",id1,id2,value);
		}

		else if(strcmp(argv[1],"-p") == 0) {
			readFile_w(f,argv[2]);
			int id1 = atoi(argv[3]);
			int id2 = atoi(argv[4]);
			int value = (int)shortestPath(graph,id1,id2,path,&length);
			printf("%d\n",value);
			for(int i=0;i<length;i++) {
				printf("%s\n",getVertex(graph,path[i]));
			}
		}
	

	else if(strcmp(argv[1],"-s") == 0) {
		readFile_w(f,argv[2]);
		int num = argc - 3;
		int arr[10];
		for(int i=0;i<num;i++) {
			arr[i] = atoi(argv[i+3]);
		}

		for(int i=0;i<num-1;i++) {
			for(int j=i+1;j<num;j++) {
				if(arr[i] > arr[j])
					swap(&arr[i],&arr[j]);
			}
		}

		printf("%d\n",num);
		for(int i=0;i<num;i++) {
			printf("%d %s\n",arr[i],getVertex(graph,arr[i]));
		}

		int count = 0;

		for(int i=0;i<num-1;i++) {
			for(int j=i+1;j<num;j++) {
				if(hasEdge(graph, arr[i],arr[j]))
					count++;
			}
		}

		printf("%d\n", count);

		for(int i=0;i<num-1;i++) {
			for(int j=i+1;j<num;j++) {
				if(hasEdge(graph,arr[i],arr[j])) {
					printf("%d %d %d\n",arr[i],arr[j],(int)getEdgeValue(graph,arr[i],arr[j]));
				}
			}
		}
	}

	return 0;
}

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void readFile_v(FILE *f, char *name) {
	f = fopen(name,"r");
	int id,id1,id2;
	double w;
	char str[30];
	fscanf(f,"%d",&numOfV);
	for(int i=0;i<numOfV;i++) {
		fscanf(f,"%d",&id);
		fscanf(f,"%s",str);
		addVertex(graph,id,str);
		printf("%d %s\n",id,str);
	}

	fscanf(f,"%d",&numOfE);
	for(int i=0;i<numOfE;i++) {
		fscanf(f,"%d %d %lf",&id1,&id2,&w);
		addEdge(graph,id1,id2,w);
	}

	fclose(f);
}

void readFile_w(FILE *f, char *name) {
	f = fopen(name,"r");
	int id,id1,id2;
	double w;
	char str[30];
	fscanf(f,"%d",&numOfV);
	for(int i=0;i<numOfV;i++) {
		fscanf(f,"%d",&id);
		fscanf(f,"%s",str);
		addVertex(graph,id,str);
		//printf("%d %s",id,str);
	}

	fscanf(f,"%d",&numOfE);
	for(int i=0;i<numOfE;i++) {
		fscanf(f,"%d %d %lf",&id1,&id2,&w);
		addEdge(graph,id1,id2,w);
	}

	fclose(f);
}


Graph createGraph()
{
    Graph graph;

    graph.edges = make_jrb();
    graph.vertices = make_jrb();

    return graph;
}

void dropGraph(Graph graph)
{
    JRB node, tree;

    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}

void addVertex(Graph graph, int id, char *name)
{
    JRB node = jrb_find_int(graph.vertices, id);

    if (node == NULL)
        jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node, tree;
    if (!hasEdge(graph, v1, v2))
    {
        //add v1->v2
        node = jrb_find_int(graph.edges, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
        //add v2->v1
        node = jrb_find_int(graph.edges, v2);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v2, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v1, new_jval_d(weight));
    }
}

double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree, node1;

    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return INFINITIVE_VALUE;
    tree = (JRB)jval_v(node->val);
    node1 = jrb_find_int(tree, v2);

    if (node1 == NULL)
        return INFINITIVE_VALUE;

    return jval_d(node1->val);
}

char *getVertex(Graph graph, int id)
{
    JRB node = jrb_find_int(graph.vertices, id);

    if (node == NULL)
        return NULL;
    else
        return jval_s(node->val);
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;

    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;

    node = jrb_find_int(graph.edges, v);

    if (node == NULL)
        return 0;

    tree = (JRB)jval_v(node->val);

    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

double shortestPath(Graph graph, int s, int t, int *path, int *length)
{
    double distance[1000], min, w, total;
    int previous[1000], tmp[1000], visited[1000];
    int n, output[100], i, v, u;
    Dllist queue, ptr, node;

    for (i = 0; i < 1000; i++)
    {
            distance[i] = INFINITIVE_VALUE;
            visited[i] = 0;
            previous[i] = 0;
    }
    distance[s] = 0;
    previous[s] = s;
    visited[s] = 1;

    queue = new_dllist();
    dll_append(queue, new_jval_i(s));

    while(!dll_empty(queue))
    {
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue)
        {
            u = jval_i(ptr->val);
            if(min > distance[u])
            {
                min = distance[u];
                node = ptr;
            }
        }
        u = jval_i(node->val);
        visited[u] = 1;
        dll_delete_node(node);

        if(u == t)
            break;

        n = outdegree(graph, u, output);

        for (i = 0; i < n; i++)
        {
            v = output[i];
            w = getEdgeValue(graph, u, v);
            if( distance[v] > distance[u] + w)
            {
                distance[v] = distance[u] + w;
                previous[v] = u;
            }
            if(visited[v] == 0)
                dll_append(queue, new_jval_i(v));
        }
    }

    total = distance[t];
    if(total != INFINITIVE_VALUE)
    {
        tmp[0] = t;
        n = 1;
        while(t != s)
        {
            t = previous[t];
            tmp[n++] = t;
        }

        for (i = n - 1; i >= 0; i--)
        {
            path[n - i - 1] = tmp[i];
        }
        *length = n;
    }

    return total;
}

int getNumOfV(Graph graph)
{
    JRB node;
    int total = 0;

    jrb_traverse(node, graph.vertices)
        total++;

    return total;
}

int getNumOfE(Graph graph)
{
    JRB node1, node2;
    int total = 0;

    jrb_traverse(node1, graph.vertices)
        jrb_traverse(node2, graph.vertices) if (hasEdge(graph, jval_i(node1->key), jval_i(node2->key)))
            total++;

    return total;
}