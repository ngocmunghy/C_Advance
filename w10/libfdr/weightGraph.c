#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h" //Double linked list

#define INFINITIVE_VALUE 10000000

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2); // return INFINITIVE_VALUE if no edge between v1 and v2
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
double shortedPath(Graph graph, int s, int t, int *path, int *length); // return the total weight of the path and the path is given via path and its length.Return INFINITIVE_VALUE if no mpath is found


void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
int hasEdge(Graph graph, int v1, int v2);
int DAG(Graph graph);
void topologicalSort(Graph graph, int *output, int *n);
void dropGraph(Graph graph);

int main()
{
    int i, s, t, length, path[1000];
    Graph g = createGraph();
    addVertex(g, 1, "V1");
    addVertex(g, 2, "V2");
    addVertex(g, 3, "V3");
    addVertex(g, 4, "V4");
    addVertex(g, 5, "V5");
    addVertex(g, 6, "V6");
    addVertex(g, 7, "V7");
    addVertex(g, 8, "V8");

    addEdge(g, 1, 2, 9);
    addEdge(g, 1, 6, 14);
    addEdge(g, 1, 7, 15);
    addEdge(g, 2, 3, 24);
    addEdge(g, 3, 5, 3);
    addEdge(g, 3, 8, 19);
    addEdge(g, 4, 3, 6);
    addEdge(g, 4, 8, 6);
    addEdge(g, 5, 4, 11);
    addEdge(g, 5, 8, 16);
    addEdge(g, 6, 3, 18);
    addEdge(g, 6, 5, 30);
    addEdge(g, 6, 7, 5);
    addEdge(g, 7, 5, 20);
    addEdge(g, 7, 8, 44);

    s = 1;
    t = 8;

    //printf("Weight of verter %d to %d is : %lf\n", 0, 1, getEdgeValue(g, 0, 1));
    
    int output[100];
    int total = outdegree(g,6,output);
    
    printf("Cac dinh ke vs dinh 6: ");
    for(int i=0;i<total;i++) {
    	printf("%4d",output[i]);
    }
    
    printf("\n");

    double weight = shortedPath(g, s, t, path, &length);
    if (weight == INFINITIVE_VALUE)
        printf("No path between %d and %d\n", s, t);
    else
    {
        printf("Path between %d and %d: ", s, t);
        for (i = 0; i < length; i++)
            printf("%4d", path[i]);
        printf("\n");
        printf("Total weight: %f\n", weight);
    }

    dropGraph(g);
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
    if(node == NULL)
        jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
}




char *getVertex(Graph graph, int id)
{
    JRB node = jrb_find_int(graph.vertices, id);

    if(node == NULL)
        return NULL;
    else
        return jval_s(node->val);
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;

    node = jrb_find_int(graph.edges, v1);
    if(node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if(jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node, tree;
    if(!hasEdge(graph, v1, v2))
    {
        node = jrb_find_int(graph.edges, v1);
        if(node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
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

    if(node1 == NULL)
        return INFINITIVE_VALUE;

    return jval_d(node1->val);
}

double shortedPath(Graph graph, int s, int t, int *path, int *length)//length: so dinh ma no di qua = so luong path
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
    distance[s] = 0; // khoang cach toi s = 0
    previous[s] = s; // truoc s la s
    visited[s] = 1; // s da duoc tham

    queue = new_dllist(); // khoi tao queue
    dll_append(queue, new_jval_i(s)); // them s vao queue

    while(!dll_empty(queue))
    {
        min = INFINITIVE_VALUE; // min = infinitive
        dll_traverse(ptr, queue) // duyet queue tu dau
        {
            u = jval_i(ptr->val); // 
            if(min > distance[u]) // neu min > khoang cach toi u
            {
                min = distance[u]; // min = distance[u]
                node = ptr; // node = ptr
            }
        }
        u = jval_i(node->val); 
        visited[u] = 1; // danh dau da tham u
        dll_delete_node(node); // xoa node

        if(u == t)
            break;

        n = outdegree(graph, u, output); // n = so luong outdegree cua u trong graph

        for (i = 0; i < n; i++)
        {
            v = output[i]; 
            w = getEdgeValue(graph, u, v);
            if( distance[v] > distance[u] + w) // neu khoang cach toi v > khoang cach toi u + w
            {
                distance[v] = distance[u] + w; // cap nhat khoang cach toi v
                previous[v] = u; // truoc v la u
            }
            if(visited[v] == 0) // neu v chua duoc tham
                dll_append(queue, new_jval_i(v)); // them v vao queue
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

int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if(jrb_find_int(tree, v))
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

    if(node == NULL)
        return 0;

    tree = (JRB)jval_v(node->val);

    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

int DAG(Graph graph)
{
    int visited[1000] = {};
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, graph.vertices)
    {
        memset(visited, 0, sizeof(visited));//copy size character 0 into size first vistied
        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack, new_jval_i(start)); /* Inserts at the end of the list */

        while(!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);

            if(!visited[u])
            {
                visited[u] = 1;
                n = outdegree(graph, u, output);
                for (i = 0; i < n; i++)
                {
                    v = output[i];
                    if (v == start) //cycle detected
                        return 0;
                    if(!visited[v])
                        dll_append(stack, new_jval_i(v));
                }
            }
        }
    }

    return 1;//no cycle
}

void topologicalSort(Graph graph, int *output, int *n)
{
    int indeg[1000], tmp[100], m, i, u, v, total;
    JRB vertex;
    Dllist node, queue;

    queue = new_dllist();
    jrb_traverse(vertex, graph.vertices)
    {
        u = jval_i(vertex->key);
        indeg[u] = indegree(graph, u, tmp);//tong cac dinh vao u, luu tru trong tmp
        if(indeg[u] == 0)//dinh ko co cung vao
            dll_append(queue, new_jval_i(u));//chen vao queue
    }
    total = 0;//chi so output cho ra topo sort
    while(!dll_empty(queue))
    {
        node = dll_first(queue);
        u = jval_i(node->val);//dinh cua node
        dll_delete_node(node);
        output[total++] = u;//cho dinh u vao list
        m = outdegree(graph, u, tmp); //tong so cung di ra tu u
        for (i = 0; i < m;i++)
        {
            v = tmp[i];//dinh ma u chi toi
            indeg[v]--;//xoa bot lien ket tu u->v
            if(indeg[v] == 0)
                dll_append(queue, new_jval_i(v));
        }
    }
    *n = total;
}
