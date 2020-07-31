#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define INFINITIVE_VALUE 9999

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
int getNumOfV(Graph graph);
int getNumOfE(Graph graph);
void dropGraph(Graph graph);
void freeEdge(Graph graph, int v1, int v2);	

int main() {

	int choice,v1,v2,output[100],length,path[10];
	char dem[30],str[50];
	char number[3],proName[20];
	FILE *file;
	Graph graph = createGraph();

	do {
		printf("\n====C_Advance 20162====\n");
		printf("\n1. In danh sach san pham\n");
		printf("2. In danh sach giao dich\n");
		printf("3. Hien thi muc do lien quan giua 2 san pham\n");
		printf("4. In danh sach cac san pham lien quan\n");
		printf("5. Hien thi moi lien he giua 2 san pham\n");
		printf("0. Thoat\n");
		printf("\nNhap lua chon cua ban: ");
		scanf("%d",&choice);
		gets(dem);

		switch(choice) {

		case 1:
		file = fopen("products.txt","r+");
		if(file == NULL) {
			printf("Can't read file.\n");
			return (-1);
		}

		while(fgets(str,30,file) != NULL) {
			int i = 0;
			
			while(str[i] != ' ') {
				number[i] = str[i];
				i++;
			}
			i++;
			number[i] = '\0';
			int num = atoi(number);
			int j = i;
			i = 0;
			while(str[j] != '\n') {
				proName[i] = str[j];
				i++;
				j++;
			}
		// i++;
			proName[i] = '\0';
			addVertex(graph,num,proName);
			printf("Ma san pham: %d \n",num);
			printf("Ten san pham: %s\n\n",proName);
		}

		fclose(file);

		break;

		case 2:
			
			file = fopen("orderhistory.txt","r+");
			if(file == NULL) {
				printf("Cannot read the file\n");
				return (-1);
			}

			while(fgets(str,30,file) != NULL) {
				char *token;
				int count = 1;
				int orderNum[10] = {0};

				token = strtok(str," ");

				while(token != NULL) {
					int num = atoi(token);
					orderNum[count-1]=num;
					printf("%s\t", getVertex(graph,num));
					token = strtok(NULL," ");
					count++;
				}	
				printf("\n");
				for(int i=0;i<count-1;i++) {
					for(int j=i+1;j<count;j++) {
						if(!hasEdge(graph,orderNum[i],orderNum[j]))
							addEdge(graph,orderNum[i],orderNum[j],1);
						else {
                            JRB node,tree;
							double value = getEdgeValue(graph,orderNum[i],orderNum[j]) + 1;
							freeEdge(graph,orderNum[i],orderNum[j]);
                            node = jrb_find_int(graph.edges, orderNum[i]);
                            tree = (JRB)jval_v(node->val);
                            jrb_insert_int(tree, orderNum[j], new_jval_d(value));

                            node = jrb_find_int(graph.edges, orderNum[j]);
                            tree = (JRB)jval_v(node->val);
                            jrb_insert_int(tree, orderNum[i], new_jval_d(value));
							//addEdge(graph,orderNum[i],orderNum[j],value);
						}
					}
				}
			}
			
			fclose(file);

			break;

		case 3:

			printf("Nhap ma so san pham 1: ");
			scanf("%d",&v1);
			printf("Nhap ma so san pham 2: ");
			scanf("%d",&v2);
			int value = (int)getEdgeValue(graph,v1,v2);
			if (value == INFINITIVE_VALUE)
				value = -1;
			printf("Muc do lien quan giua 2 san pham: %d\n", value);
			break;

		case 4:
			printf("Nhap ma so san pham: ");
			scanf("%d",&v1);
			int total = outdegree(graph,v1,output);
			for(int i=0;i<total-1;i++) {
				for(int j=i+1;j<total;j++) {
					if(getEdgeValue(graph,v1,output[i]) < getEdgeValue(graph,v1,output[j]))
						output[i] = output[j];
				}
			}

			printf("Cac san pham lien quan: ");
			for(int i=0;i<total;i++) {
				printf("%s\t",getVertex(graph,output[i]));
			}
			printf("\n");
			break;

		case 5:

			printf("Nhap ma so san pham thu nhat: ");
			scanf("%d",&v1);
			printf("Nhap ma so san pham thu hai: ");
			scanf("%d",&v2);
			if(shortestPath(graph,v1,v2,path,&length) == INFINITIVE_VALUE) {
				printf("Hai san pham nay khong co lien he voi nhau\n");
				break;
			}
			for(int i=0;i<length-1;i++) {
				printf("%s - ", getVertex(graph,path[i]));
			}
			printf("%s\n",getVertex(graph,path[length-1]));

			break;
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

void freeEdge(Graph graph, int v1, int v2) {
	JRB node, tree,node2;
	node = jrb_find_int(graph.edges,v1);
	tree = (JRB)jval_v(node->val);
	node2 = jrb_find_int(tree,v2);
	jrb_delete_node(node2);
	//jrb_delete_node(node);


	node = jrb_find_int(graph.edges,v2);
	tree = (JRB)jval_v(node->val);
	node2 = jrb_find_int(tree,v1);
	jrb_delete_node(node2);
	//jrb_delete_node(node);
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node, tree;
    //if (!hasEdge(graph, v1, v2))
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
