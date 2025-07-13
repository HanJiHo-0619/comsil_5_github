#include <stdio.h>
#include <stdlib.h>

#define NUM_VERTICES 7

struct node {
    int vertex;
    int weight;
    struct node *link;
};
typedef struct node* nodePointer;

// 함수 선언
void addNode(nodePointer *graph, int u, int v, int w);
void printGraph(nodePointer *graph);
void freeGraph(nodePointer *graph);

int main() {
    int u, v, w;
    nodePointer *graph;

    // 3.1 adjacency list 메모리 할당 및 초기화
    graph = (nodePointer *)malloc(sizeof(nodePointer) * NUM_VERTICES);
    for (int i = 0; i < NUM_VERTICES; i++) {
        graph[i] = NULL;
    }

    // 테스트 edge 입력
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

    for (int i = 0; i < num_edges; i++) {
        u = edges[i][0];
        v = edges[i][1];
        w = edges[i][2];
        addNode(graph, u, v, w);
    }

    // 출력 확인
    printGraph(graph);

    // 메모리 해제
    freeGraph(graph);
    return 0;
}

// 3.2 adjacency list node 추가
void addNode(nodePointer *graph, int u, int v, int w) {
    if (u == v) return;  // self loop 제거

    nodePointer p = graph[u];
    while (p != NULL) {
        if (p->vertex == v) return; // 중복 edge 제거
        p = p->link;
    }

    // head insert
    nodePointer newNode = (nodePointer)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->link = graph[u];
    graph[u] = newNode;
}
// 출력 함수 (채점용)
void printGraph(nodePointer *graph) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        printf("%d", i);
        nodePointer p = graph[i];
        while (p != NULL) {
            printf(" (%d %d %d)", i, p->vertex, p->weight);
            p = p->link;
        }
        printf("\n");
    }
}

// 3.3 adjacency list 메모리 해제
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