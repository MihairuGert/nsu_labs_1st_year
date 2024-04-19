#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES_NUMBER 2000
#define BIT_COUNT 8

typedef unsigned char u_char;

typedef struct Stack {
    int index;
    short* array;
} Stack;


bool PrintCheckInput(int vertices_number, int edges_number) {
    if (vertices_number > MAX_VERTICES_NUMBER || vertices_number < 0) {
        printf("bad number of vertices");
        return false;
    } else if (edges_number > (vertices_number * (vertices_number + 1) / 2) || edges_number < 0) {
        printf("bad number of edges");
        return false;
    }
    return true;
}

u_char* CreateMatrix(int vertices_number) {
    const int size = sizeof(u_char) * (vertices_number + 1) * (vertices_number / BIT_COUNT + 1);
    u_char* matrix = malloc(size);
    for (int i = 0; i < size; i++) {
        matrix[i] = 0;
    }
    return matrix;
}

void DestroyMatrix(u_char* matrix) {
    free(matrix);
}

u_char GetMask(int to) {
    u_char bit = to % 8;
    u_char res = 1;
    res = res << (BIT_COUNT - bit - 1);
    return res;
}

bool IsEdge(u_char byte_to_check, int to) {
    return GetMask(to) & byte_to_check;
}

void SetMatrixElement(u_char* matrix, int from, int to, int vertices_number) {
    int byte = (int)(to / BIT_COUNT);
    // Index of u_char where needed bit locates.
    int index = from * (int)(vertices_number / BIT_COUNT + 1) + byte;
    matrix[index] = matrix[index] | GetMask(to);
}

bool GetMatrixElement(u_char* matrix, int from, int to, int vertices_number) {
    int byte = (int)(to / BIT_COUNT);
    int index = from * (int)(vertices_number / BIT_COUNT + 1) + byte;
    return IsEdge(matrix[index], to);
}

// Prints a mistake if it exists and then returns false. 
// Unless prints nothing and returns true.
bool ScanAndProcessGraph(u_char* matrix, int vertices_number, int edges_number) {
    int from, to;
    for (int i = 0; i < edges_number; i++) {
        if (scanf("%d %d", &from, &to) < 2) {
            printf("bad number of lines");
            return false;
        } else if (from < 1 || from > vertices_number || to < 1 || to > vertices_number) {
            printf("bad vertex");
            return false;
        }
        SetMatrixElement(matrix, from, to, vertices_number);
    }
    return true;
}

bool TopSortInternal(u_char* matrix, Stack* stack, u_char* is_visited, int vertex, int vertices_number) {
    bool flag = true;
    is_visited[vertex] = 1;
    for (int i = vertices_number; i >= 1; i--) {
        if (i == vertex) {
            continue;
        }
        if (GetMatrixElement(matrix, vertex, i, vertices_number) && is_visited[i] == 1) {
            return false;
        }
        if (GetMatrixElement(matrix, vertex, i, vertices_number)) {
            if (is_visited[i] != 2) {
                flag = TopSortInternal(matrix, stack, is_visited, i, vertices_number);
            }
        }
    }
    is_visited[vertex] = 2;
    stack->array[stack->index] = vertex;
    stack->index++;
    return flag;
}

void TopSort(u_char* matrix, Stack* stack, int vertices_number) {
    u_char* is_visited = malloc((vertices_number + 1) * sizeof(u_char));
    for (int i = 0; i < vertices_number + 1; i++) {
        is_visited[i] = 0;
    }
    stack->index = 0;
    for (int i = 1; i < vertices_number + 1; i++) {
        if (is_visited[i] == 0) {
            if (!TopSortInternal(matrix, stack, is_visited, i, vertices_number)) {
                free(is_visited);
                stack->index = -1;
                return;
            }
        }
    }
    free(is_visited);
    stack->index = stack->index - 1;
}

int main(void) {
    int vertices_number, edges_number; 
    if (!scanf("%d", &vertices_number)) {
        return 0;
    }
    if (scanf("%d", &edges_number) < 1) {
        printf("bad number of lines");
        return 0;
    }
    if (!PrintCheckInput(vertices_number, edges_number)) {
        return 0;
    }
    u_char* matrix = CreateMatrix(vertices_number);
    Stack stack;
    stack.array = malloc((vertices_number + 1) * sizeof(short));
    if (!ScanAndProcessGraph(matrix, vertices_number, edges_number)) {
        DestroyMatrix(matrix);
        free(stack.array);
        return 0;
    }
    TopSort(matrix, &stack, vertices_number);
    if (stack.index == -1) {
        printf("impossible to sort");
        DestroyMatrix(matrix);
        free(stack.array);
        return 0;
    }
    for (int i = stack.index; i >= 0; i--) {
        printf("%d ", stack.array[i]);
    }
    DestroyMatrix(matrix);
    free(stack.array);
    return EXIT_SUCCESS;
}
