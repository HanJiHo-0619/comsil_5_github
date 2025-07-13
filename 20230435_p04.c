#include <stdio.h>
#include <stdlib.h>

#define NUM_VERTICES 7
#define INVALID_COMPONENT 0

struct node {
    int vertex;
    int weight;
    struct node *link;
};
typedef struct node* nodePointer;

int comp[NUM_VERTICES];  // component label array

// 함수 선언
void addNode(nodePointer *graph, int u, int v, int w);
void printComponents(int numComponents);
void freeGraph(nodePointer *graph);
void dfsl(nodePointer *graph, int start, int label);
int countAndLabel(nodePointer *graph);

int main() {
    int edges[][3] = {
        {0, 5, 10},
        {1, 6, 14},
        {2, 3, 12},
        {3, 4, 22},
        {3, 2, 12},
        {4, 3, 22},
        {5, 0, 10},
        {6, 1, 14}
    };
    int num_edges = sizeof(edges) / sizeof(edges[0]);

    nodePointer *graph = (nodePointer *)malloc(sizeof(nodePointer) * NUM_VERTICES);
    for (int i = 0; i < NUM_VERTICES; i++)
        graph[i] = NULL;

    for (int i = 0; i < num_edges; i++) {
        int u = edges[i][0], v = edges[i][1], w = edges[i][2];
        addNode(graph, u, v, w);
    }

    int numComponents = countAndLabel(graph);
    printf("%d\n", numComponents);
    printComponents(numComponents);

    freeGraph(graph);
    return 0;
}

// 연결 리스트 삽입 함수 (중복 제거, self-loop 제거)
void addNode(nodePointer *graph, int u, int v, int w) {
    if (u == v) return;

    nodePointer p = graph[u];
    while (p != NULL) {
        if (p->vertex == v) return; // 중복 제거
        p = p->link;
    }

    nodePointer newNode = (nodePointer)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->link = graph[u];
    graph[u] = newNode;
}

// DFS로 component labeling=(이거 구현이 문제)
void dfsl(nodePointer *graph, int start, int label) {
    comp[start] = label;
    nodePointer ptr = graph[start];
    while (ptr != NULL) {
        int next = ptr->vertex;
        if (comp[next] == INVALID_COMPONENT) {
            dfsl(graph, next, label);
        }
        ptr = ptr->link;
    }
}

// 전체 component 탐색 (이거 구현이 문제)
int countAndLabel(nodePointer *graph) {
    int count = 0;

    for (int v = 0; v < NUM_VERTICES; v++)
        comp[v] = INVALID_COMPONENT;

    for (int v = 0; v < NUM_VERTICES; v++) {
        if (comp[v] == INVALID_COMPONENT) {
            count++;
            dfsl(graph, v, count);
        }
    }

    return count;
}

// 결과 출력 함수
void printComponents(int numComponents) {
    for (int c = 1; c <= numComponents; c++) {
        for (int v = 0; v < NUM_VERTICES; v++) {
            if (comp[v] == c)
                printf("%d ", v);
        }
        printf("\n");
    }
}

// 그래프 메모리 해제
void freeGraph(nodePointer *graph) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        nodePointer p = graph[i];
        while (p != NULL) {
            nodePointer temp = p;
            p = p->link;
            free(temp);
        }
    }
    free(graph);
}