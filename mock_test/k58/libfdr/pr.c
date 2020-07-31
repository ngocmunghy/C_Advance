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
JRB node;
int numOfV,numOfE;
int output[10],path[10],length;
char str[30];
int id,id1,id2,i,j,k,total,total1,total2;
int count,output1[10],output2[10],output3[10];

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
void readFile(FILE *f, char *filename);

Graph graph;

int main(int argc, char *argv[]) {
    graph = createGraph();

	if(strcmp(argv[1],"help") == 0) {
        printf("Support command: help, i2n, n2i, cc, cr, odg, rel.\n");
    }

    else if(strcmp(argv[1],"about") == 0 ) {
        printf("C advanced 20142 final exam.\n");
    }

    else if(strcmp(argv[1],"i2n") == 0) {
        readFile(f, argv[2]);
        id = atoi(argv[3]);
        printf("%s\n",getVertex(graph,id) );
    }

    else if(strcmp(argv[1],"n2i") == 0) {
        readFile(f,argv[2]);
        jrb_traverse(node,graph.vertices) {
            strcpy(str,(char*)jval_v(node->val));
            if(strcmp(str,argv[3]) == 0){
                printf("%d\n", jval_i(node->key));
                break;
            }
        }
    }

    else if(strcmp(argv[1],"cc") == 0) {
        readFile(f,argv[2]);
        total1 = outdegree(graph,atoi(argv[3]),output1);
        total2 = outdegree(graph,atoi(argv[4]),output2);
        count = 0;
        for(i=0;i<total1;i++) {
            for(j=0;j<total2;j++) {
                if(output1[i] == output2[j]) {
                    output3[count] = output2[j];
                    //printf("%s\n",getVertex(graph,output1[i]));
                    count++;
                }
            }
        }

        if(count == 0) {
            printf("No co-reference!\n");
        } else {
            for(i=0;i<count;i++) {
                printf("%s\n",getVertex(graph,output3[i]));
            }
        }
    }

    else if(strcmp(argv[1],"odg") == 0) {
        readFile(f,argv[2]);
        count = atoi(argv[3]);
        jrb_traverse(node,graph.vertices) {
            total = outdegree(graph,jval_i(node->key),output);
            if(total > count) {
                printf("%s\n",getVertex(graph,jval_i(node->key)));
            }
        }
    }

    else if(strcmp(argv[1],"rel") == 0) {
        readFile(f,argv[2]);
        id = atoi(argv[3]);
        total = outdegree(graph,id,output);
        for(i=0;i<total;i++) {
            printf("%s\n",getVertex(graph,output[i]) );
            total1 = outdegree(graph,output[i],output1);
            for(j=0;j<total1;j++) {
                printf("%s\n",getVertex(graph,output1[j]) );
            }
        }
    }

	return 0;
}

void readFile(FILE *f, char *filename) {
    f = fopen(filename,"r");

    fscanf(f,"%d %d", &numOfV, &numOfE);
    for(i=0;i<numOfV;i++) {
        fscanf(f,"%s %d",str,&id);
        addVertex(graph,id,str);
    }
    for(i=0;i<numOfE;i++) {
        fscanf(f,"%d %d",&id1,&id2);
        addEdge(graph,id1,id2,1);
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
        // node = jrb_find_int(graph.edges, v2);
        // if (node == NULL)
        // {
        //     tree = make_jrb();
        //     jrb_insert_int(graph.edges, v2, new_jval_v(tree));
        // }
        // else
        // {
        //     tree = (JRB)jval_v(node->val);
        // }
        // jrb_insert_int(tree, v1, new_jval_d(weight));
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