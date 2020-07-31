#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

#define INFINITIVE_VALUE 9999.0

typedef struct {
	JRB edges;
	JRB vertices;
} Graph;

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
double shortestPath_5(Graph graph, int s, int t, int *path, int *length);
int getNumOfV(Graph graph);
int getNumOfE(Graph graph);

int DAG(Graph graph);
void topologicalSort(Graph graph, int *output, int *n);
void dropGraph(Graph graph);



int main() {
	Graph graph = createGraph();
    int s,t;
    int path[10];
    int length;
	int i,j,k,m,id1,id2;
    int max3b=0;
	double weight;
	int numOfV,numOfE,choice;
    int output[10];
	FILE *f;
	do{
		printf("1. Doc ban do\n");
		printf("2. In danh sach lien ke moi thanh tri.\n");
		printf("3. Dua ra\n");
		printf("4. Tim duong di ngan nhat.\n");
        printf("5. Tim duong di bo ngan nhat.\n");
		printf("0. Thoat\n");
		printf("Nhap lua chon cua ban: ");
		scanf("%d",&choice);
		switch(choice) {
			case 1: 

				f = fopen("dothi.txt","r");
				if(f == NULL) {
					printf("Cannot open the file.\n");
				}
				else {
					fscanf(f,"%d %d", &numOfV, &numOfE);
					for(i=0;i<numOfV;i++) {
						addVertex(graph, i+1, "");
					}		
					for(i=0; i<numOfE;i++) {
						fscanf(f,"%d %d %lf\n", &id1, &id2, &weight);
						addEdge(graph, id1, id2,weight);
					}
				}
				//numOfV = getNumOfV(graph);

				printf("Ma tran ke cua do thi:\n");

                for( i = 0; i < numOfV; i++) {
                    for( j = 0; j < numOfV; j++) {
                        if(hasEdge(graph, i+1, j+1)) {
                            printf("\t%.0lf", getEdgeValue(graph,i+1,j+1));
                        }
                        else
                            printf("\t0");
                    }
                    printf("\n");
                }
                printf("\n");

				break;
			case 2:

                for(i=1;i<=numOfV;i++) {
                    printf("\tCastle %d:", i);
                    for(j=1;j<=numOfV;j++) {
                        if(hasEdge(graph,i,j)) {
                            printf("\t%d",j);
                        }
                    }
                    printf("\n");
                }
                printf("\n");
				break;
			case 3:
                //int outdegree(Graph graph, int v, int *output);
                
                printf("\n\t3.a ");
                printf("Nhung thanh tri chi co the truc tiep den no bang cach di bo:");
                for(i=1;i<=numOfV;i++) {
                    int max = outdegree(graph,i,output);
                    for(j=0;j<max;j++) {
                        if(getEdgeValue(graph,i,output[j]) < 50)
                            break;
                    }
                    if(j == max)
                        printf("\t%d",i);
                }
                printf("\n");
                

                printf("\n\t3.b ");
                printf("Nhung thanh tri co nhieu duong di noi truc tiep den no nhat:");
                for(i=1;i<=numOfV;i++) {
                    if(max3b < indegree(graph,i,output))
                        max3b = indegree(graph,i,output);
                }
                for(i=1;i<=numOfV;i++) {
                    if(indegree(graph,i,output) == max3b)
                        printf("\t%d",i);
                }
                printf("\n\n");
				break;
			case 4:
            //double shortestPath(Graph graph, int s, int t, int *path, double *length);

                printf("\t-Nhap thanh tri xuat phat s: ");
                scanf("%d",&s);
                printf("\t-Nhap thanh tri xuat phat t: ");
                scanf("%d",&t);
                printf("\t-Duong di ngan nhat tu %d toi %d: ",s,t);
                if (shortestPath(graph,s,t,path,&length) == INFINITIVE_VALUE) {
                    printf("\n\t\tROUTE NOT FOUND\n");
                    break;
                }

                printf("\n\t\t+ Do dai: %.0lf",shortestPath(graph,s,t,path,&length));
                printf("\n\t\t+ Doan duong phai di: ");
                for(i=0;i<length-1;i++) {
                    printf("%d->",path[i]);
                }
                printf("%d", path[length-1]);

                printf("\n\n");
				break;

            case 5:
                printf("\t-Nhap thanh tri xuat phat s: ");
                scanf("%d",&s);
                printf("\t-Nhap thanh tri xuat phat t: ");
                scanf("%d",&t);
                printf("\t-Duong di ngan nhat tu %d toi %d: ",s,t);
                if (shortestPath_5(graph,s,t,path,&length) == INFINITIVE_VALUE) {
                    printf("\n\t\tROUTE NOT FOUND\n");
                    break;
                }

                printf("\n\t\t+ Do dai: %.0lf",shortestPath_5(graph,s,t,path,&length));
                printf("\n\t\t+ Doan duong phai di: ");
                for(i=0;i<length-1;i++) {
                    printf("%d->",path[i]);
                }
                printf("%d", path[length-1]);

                printf("\n\n");
                break;
			case 0:
				printf("Cam on ban da su dung.\n");
				break;
			default:
				printf("Khong ton tai lua chon.\n");
		}
	} while(choice != 0);
	return 0;
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

double shortestPath_5(Graph graph, int s, int t, int *path, int *length)
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
            if ((min > distance[u] && distance[u] >= 50) || distance[u] == 0)
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
            if(w < 50)
                continue;
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