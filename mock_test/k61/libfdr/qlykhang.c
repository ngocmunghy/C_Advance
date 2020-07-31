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

typedef struct {
    char ten[30];
    int id;
    int soluong;
} SP;

typedef struct {
    char ten[30];
    int id;
    SP mathang[100];
} KH;


FILE *f;
int numOfPro = 0;
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
void readFile_s(FILE *f, char *name, SP *sp);
void readFile_s_none(FILE *f, char *name, SP *sp);
void readFile_w(FILE *f, char *name, SP *sp, KH *kh);
void readFile_a(FILE *f, char *name, SP *sp, KH *kh);

Graph graph;

int main(int argc, char* argv[]) {
	graph = createGraph();
    SP sp[100];
    KH kh[100];
	
	if(strcmp(argv[1],"-t") == 0) {
		printf("C-Advanced, HK20182\n");
	}

	else if(strcmp(argv[1],"-s") == 0) {
		readFile_s(f,argv[2],sp);
	}

    else if(strcmp(argv[1],"-w") == 0) {
        readFile_s_none(f,argv[3],sp);
        readFile_w(f,argv[2],sp,kh);
        int id1 = atoi(argv[4]);
        int id2 = atoi(argv[5]);
        JRB node1, node2;
        node1 = jrb_find_int(graph.vertices,id1);
        node2 = jrb_find_int(graph.vertices,id2);
        // printf("%d\n",(int)getEdgeValue(graph,id1,id2));
        if(node1 == NULL || node2 == NULL) {
            printf("-1\n");
            return 0;
        }
        else {
            int value = (int)getEdgeValue(graph,id1,id2);
            if(value == INFINITIVE_VALUE) {
                printf("-1\n");
                return 0;
            } 
            printf("%d km\n",value);
        }
    }

    else if(strcmp(argv[1],"-a") == 0) {
        readFile_s_none(f,argv[3],sp);
        readFile_a(f,argv[2],sp,kh);   
    }

    else if(strcmp(argv[1],"-h") == 0) {
        readFile_s_none(f,argv[3],sp);
        readFile_w(f,argv[2],sp,kh);   
        int i,j,k,id = atoi(argv[4]);
        int idKH = atoi(argv[5]);
        char nameSP[30],nameKH[30];
        for(i=0;i<numOfV;i++) {
            if(idKH == kh[i].id)
                break;
        }

        printf("%s\n", kh[i].ten); // KhohangC
        for(j=0;j<numOfPro;j++) {
            if(id == kh[i].mathang[j].id) 
                break;
        }

        printf("%s %d\n",kh[i].mathang[j].ten,kh[i].mathang[j].soluong); // Tulanh 0
        printf("---\n");
        printf("Cac kho ke la: \n");
        int total = outdegree(graph,idKH,output);
        for(k=0;k<total;k++) {
            strcpy(nameKH,getVertex(graph,output[k]));
            int m;
            for(m=0;m<numOfPro;m++) {
                if(output[k] == kh[m].id)
                    break;
            }
            printf("%s\n",nameKH);
            printf("%s %d\n",kh[i].mathang[j].ten,kh[m].mathang[j].soluong);
        }
    }

	return 0;
} 

void readFile_s(FILE *f,char *name, SP *sp) {
	f = fopen(name,"r+");
	int id;
	char str[30];
	fscanf(f,"%d",&numOfPro);
	for(int i=0;i<numOfPro;i++) {
		fscanf(f,"%s",str);
		fscanf(f,"%d",&id);
		printf("%s %d\n",str,id);
        strcpy(sp[i].ten,str);
        sp[i].id = id;
	}
    fclose(f);
}

void readFile_s_none(FILE *f,char *name, SP *sp) {
    f = fopen(name,"r+");
    int id;
    char str[30];
    fscanf(f,"%d",&numOfPro);
    for(int i=0;i<numOfPro;i++) {
        fscanf(f,"%s",str);
        fscanf(f,"%d",&id);
        //printf("%s %d\n",str,id);
        strcpy(sp[i].ten,str);
        sp[i].id = id;
    }
    fclose(f);
}

void readFile_w(FILE *f, char *name, SP *sp, KH *kh) {
    f = fopen(name,"r");
    int id1,id2;
    double w;
    int k,quantity;
    int idSP, idKH;
    char nameSP[30], nameKH[30];
    fscanf(f,"%d",&numOfV);

    for(int i=0;i<numOfV;i++) {
        fscanf(f,"%s %d",nameKH,&idKH);
        strcpy(kh[i].ten,nameKH);
        kh[i].id = idKH;
        addVertex(graph, idKH,nameKH);

        for(int j=0;j<numOfPro;j++) {
            fscanf(f,"%d %d",&idSP,&quantity);
            kh[i].mathang[j].soluong = quantity;
            kh[i].mathang[j].id = idSP;
            for(k=0;k<numOfPro;k++) {
                if(idSP == sp[k].id)
                    break;
            }
            strcpy(kh[i].mathang[j].ten,sp[k].ten);
        }
    }

    fscanf(f,"%d",&numOfE);

    for(int i=0;i<numOfE;i++) {
        fscanf(f,"%d %d %lf", &id1,&id2,&w);
        addEdge(graph,id1,id2,w);
    }

    fclose(f);

}

void readFile_a(FILE *f, char *name, SP *sp, KH *kh) {
    f = fopen(name,"r");
    int id1,id2;
    double w;
    int quantity;
    int idSP, idKH;
    char nameSP[30], nameKH[30];
    fscanf(f,"%d",&numOfV);

    for(int i=0;i<numOfV;i++) {
        fscanf(f,"%s %d",nameKH,&idKH);
        strcpy(kh[i].ten,nameKH);
        kh[i].id = idKH;
        addVertex(graph, idKH,nameKH);
        printf("%s\n",nameKH);

        for(int j=0;j<numOfPro;j++) {
            fscanf(f,"%d %d",&idSP,&quantity);
            kh[i].mathang[j].soluong = quantity;
            kh[i].mathang[j].id = idSP;
            int k;
            for(k=0;k<numOfPro;k++) {
                if(idSP == sp[k].id)
                    break;
            }

            strcpy(kh[i].mathang[j].ten,sp[k].ten);
            printf("%s %d\n",kh[i].mathang[j].ten, quantity);
        }
        if(i<numOfV-1) {
           printf("----\n"); 
        }
        
    }

    fscanf(f,"%d",&numOfE);

    for(int i=0;i<numOfE;i++) {
        fscanf(f,"%d %d %lf", &id1,&id2,&w);
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