#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF 1000000

// 그래프 노드 구조체
struct node {
    int vertex;
    int weight;
    struct node *link;
};
typedef struct node* nodePointer;

nodePointer graph[MAX_VERTICES];
int comp[MAX_VERTICES] = {0};
int numVertex = 7;

// 입력 그래프의 모든 간선 저장 (초기에 addEdge 하지 않음)
struct edge {
    int u, v, w;
};
struct edge graphEdges[] = {
    {0, 5, 10},
    {1, 6, 14},
    {2, 3, 12},
    {3, 4, 22},
    {4, 5, 25},
    {2, 1, 16},
    {6, 4, 1},
    {2, 0, 28}
};
int numEdges = sizeof(graphEdges) / sizeof(graphEdges[0]);

struct edge mst[MAX_VERTICES];
int mstCount = 0;

// 함수 선언
void addEdge(int u, int v, int w);
void countAndLabel(int v, int label);
int reLabelComponents();
void findAndAddSafeEdges();
void printMST();
void freeGraph();

int main() {
    // 초기: F = (V, ∅)
    int count = reLabelComponents();  // 각 vertex는 자기 자신이 속한 트리

    // Sollin’s algorithm
    while (count > 1) {
        findAndAddSafeEdges();        // 각 트리마다 safe edge 1개 추가
        count = reLabelComponents();  // forest 재구성
    }

    printMST();      // 간선 추가 순서 출력
    freeGraph();     // 메모리 해제
    return 0;
}

// 무방향 간선 추가
void addEdge(int u, int v, int w) {
    nodePointer new1 = (nodePointer)malloc(sizeof(struct node));
    new1->vertex = v; new1->weight = w; new1->link = graph[u];
    graph[u] = new1;

    nodePointer new2 = (nodePointer)malloc(sizeof(struct node));
    new2->vertex = u; new2->weight = w; new2->link = graph[v];
    graph[v] = new2;
}

// DFS로 component label 부여
void countAndLabel(int v, int label) {
    comp[v] = label;
    nodePointer p = graph[v];
    while (p != NULL) {
        if (comp[p->vertex] == 0)
            countAndLabel(p->vertex, label);
        p = p->link;
    }
}

// 전체 re-label
int reLabelComponents() {
    for (int i = 0; i < numVertex; i++) comp[i] = 0;
    int label = 1;
    for (int v = 0; v < numVertex; v++) {
        if (comp[v] == 0)
            countAndLabel(v, label++);
    }
    return label - 1;
}

// 각 컴포넌트에서 safe edge 찾고 forest에 추가
void findAndAddSafeEdges() {
    struct edge minEdge[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) minEdge[i].w = INF;

    for (int i = 0; i < numEdges; i++) {
        int u = graphEdges[i].u;
        int v = graphEdges[i].v;
        int w = graphEdges[i].w;

        if (comp[u] != comp[v]) {
            int c = comp[u];
            if (w < minEdge[c].w || (w == minEdge[c].w && u < minEdge[c].u)) {
                minEdge[c] = (struct edge){u, v, w};
            }
        }
    }

    for (int i = 1; i < MAX_VERTICES; i++) {
        if (minEdge[i].w != INF) {
            int u = minEdge[i].u;
            int v = minEdge[i].v;
            int w = minEdge[i].w;

            // 중복 추가 방지
            int dup = 0;
            for (int j = 0; j < mstCount; j++) {
                if ((mst[j].u == u && mst[j].v == v) ||
                    (mst[j].u == v && mst[j].v == u)) {
                    dup = 1;
                    break;
                }
            }

            if (!dup) {
                addEdge(u, v, w);                    // forest에 간선 추가
                mst[mstCount++] = (struct edge){u, v, w};  // MST 목록에도 기록
            }
        }
    }
}

// MST 출력
void printMST() {
    for (int i = 0; i < mstCount; i++) {
        printf("(%d %d %d)\n", mst[i].u, mst[i].v, mst[i].w);
    }
}

// 그래프 메모리 해제
void freeGraph() {
    for (int i = 0; i < numVertex; i++) {
        nodePointer p = graph[i];
        while (p) {
            nodePointer temp = p;
            p = p->link;
            free(temp);
        }
    }
}