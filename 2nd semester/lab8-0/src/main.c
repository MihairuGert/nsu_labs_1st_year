#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NO_EDGE -1

typedef struct Edge {
    int weigth;
    short begin, end;
} Edge;

typedef struct DisjointSetUnion {
    int* rank;
    short* parent;
} DisjointSetUnion;

// If correct prints nothing.
bool PrintCheckInput(short vertices_count, int edges_count) {
    if (vertices_count < 0 || vertices_count > 5000) {
        printf("bad number of vertices");
        return false;
    }
    if (edges_count < 0 || edges_count > (vertices_count * (vertices_count + 1) / 2)) {
        printf("bad number of edges");
        return false;
    }
    return true;
}

bool ScanAndProcessGraph(Edge* edge_array, short vertices_count, int edges_count) {
    for (int i = 0; i < edges_count; i++) {
        unsigned int weigth;
        short begin, end;
        if (scanf("%hi %hi %u", &begin, &end, &weigth) < 3) {
            printf("bad number of lines");
            return false;
        }
        if (begin < 1 || begin > vertices_count || end < 1 || end > vertices_count) {
            printf("bad vertex");
            return false;
        }
        if (weigth > INT_MAX) {
            printf("bad length");
            return false;
        }
        edge_array[i].begin = begin;
        edge_array[i].end = end;
        edge_array[i].weigth = weigth;
    }
    return true;
}

void Swap(Edge* element1, Edge* element2) {
    Edge c = *element1;
    *element1 = *element2;
    *element2 = c;
}

void QuickSort(Edge* input_array, int size) {
    int size_buffer = size - 1;
    Edge central_element = input_array[size_buffer / 2];
    int counter = 0;
    do {
        while (input_array[counter].weigth < central_element.weigth) {
            ++counter;
        }
        while (input_array[size_buffer].weigth > central_element.weigth) {
            --size_buffer;
        }

        if (counter <= size_buffer) {
            Swap(&input_array[size_buffer], &input_array[counter]);
            ++counter;
            --size_buffer;
        }
    } while (counter <= size_buffer);
    if (size_buffer > 0) {
        QuickSort(input_array, size_buffer + 1);
    }
    if (size > counter) {
        QuickSort(input_array + counter, size - counter);
    }
}

void CreateSet(DisjointSetUnion* set, short vertices_count) {
    set->parent = malloc((vertices_count + 1) * sizeof(short));
    set->rank = malloc((vertices_count + 1) * sizeof(int));
    for (int i = 0; i < vertices_count + 1; i++) {
        set->parent[i] = i;
    }
    for (int i = 0; i < vertices_count + 1; i++) {
        set->rank[i] = 0;
    }
}

short FindParent(DisjointSetUnion* set, short vertex) {
    if (set->parent[vertex] == vertex) {
        return vertex;
    }
    return FindParent(set, set->parent[vertex]);
}

void UnionSet(DisjointSetUnion* set, short begin, short end) {
    if (set->rank[begin] > set->rank[end]) {
        set->parent[end] = begin;
    } else if (set->rank[begin] < set->rank[end]) {
        set->parent[begin] = end;
    } else {
        set->parent[end] = begin;
        set->rank[begin]++;
    }
}

void DestroySet(DisjointSetUnion* set) {
    free(set->parent);
    free(set->rank);
}

bool FindMinCostSpanningTree(Edge* edge_array, short vertices_count, int edges_count) {
    if (edges_count != 0) {
        QuickSort(edge_array, edges_count);
    }
    DisjointSetUnion set;
    int tree_index = 1;
    CreateSet(&set, vertices_count);
    for (int i = 0; i < edges_count; i++) {
        short begin_parent = FindParent(&set, edge_array[i].begin);
        short end_parent = FindParent(&set, edge_array[i].end);
        if (begin_parent != end_parent) {
            UnionSet(&set, begin_parent, end_parent);
            tree_index++;
        } else {
            edge_array[i].weigth = NO_EDGE;
        }
    }
    DestroySet(&set);
    return tree_index == vertices_count;
}

int main(void) {
    short vertices_count;  
    int edges_count;   
    if (!scanf("%hi", &vertices_count)) {
        return 0;
    }
    if (!scanf("%d", &edges_count)) {
        return 0;
    }
    if (!PrintCheckInput(vertices_count, edges_count)) {
        return 0;
    }
    Edge* edge_array = malloc(sizeof(Edge) * edges_count);
    if (!ScanAndProcessGraph(edge_array, vertices_count, edges_count)) {
        free(edge_array);
        return 0;
    }
    bool does_exist = FindMinCostSpanningTree(edge_array, vertices_count, edges_count);
    if (does_exist) {
        for (int i = 0; i < edges_count; i++) {
            if (edge_array[i].weigth != NO_EDGE) {
                printf("%d %d\n", edge_array[i].begin, edge_array[i].end);
            }
        }
    } else {
        printf("no spanning tree");
    }
    free(edge_array);
    return EXIT_SUCCESS;
}
