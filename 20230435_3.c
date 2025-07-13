#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

struct node {
    int vertex;
    int weight;
    struct node *link;
};
typedef struct node* nodePointer;

nodePointer graph[MAX_VERTICES];
int comp[MAX_VERTICES] = {0};

void addEdge(int, int, int);
void printGraph(int n);
void countAndLabel(int, int);

void main() {
    int label = 1;  /* component label */
    int numVertex = 7;  /* number of vertices in the input graph */

    /* input graph */
    addEdge(0,5,10);
    addEdge(1,6,14);
    addEdge(2,3,12);
    addEdge(3,4,22);

    printGraph(numVertex);

    // output connected components
    for (int v = 0; v < numVertex; v++) {
        if (comp[v] == 0) {
            countAndLabel(v, label);
            label++;
        }
    }

    for (int l = 1; l < label; l++) {
        printf("component %d:", l);
        for (int v = 0; v < numVertex; v++) {
            if (comp[v] == l)
                printf(" %d", v);
        }
        printf("\n");
    }

    // free memory
    for (int i = 0; i < numVertex; i++) {
        nodePointer p = graph[i];
        while (p) {
            nodePointer temp = p;
            p = p->link;
            free(temp);
        }
    }
}

void addEdge(int u, int v, int w) {  /* edge (u, v) has weight w */
    nodePointer new1 = (nodePointer)malloc(sizeof(struct node));
    new1->vertex = v;
    new1->weight = w;
    new1->link = graph[u];
    graph[u] = new1;

    nodePointer new2 = (nodePointer)malloc(sizeof(struct node));
    new2->vertex = u;
    new2->weight = w;
    new2->link = graph[v];
    graph[v] = new2;
}  

void printGraph(int n) {  /* n: number of vertices in graph G */
    for (int i = 0; i < n; i++) {
        printf("%d", i);
        nodePointer p = graph[i];
        while (p) {
            printf(" (%d %d %d)", i, p->vertex, p->weight);
            p = p->link;
        }
        printf("\n");
    }
}

void countAndLabel(int v, int count) {
    comp[v] = count;
    nodePointer p = graph[v];
    while (p) {
        if (comp[p->vertex] == 0)
            countAndLabel(p->vertex, count);
        p = p->link;
    }
}