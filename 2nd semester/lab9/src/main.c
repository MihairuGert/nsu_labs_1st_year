#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#include <stdbool.h>
#include <string.h>

typedef unsigned long long ull;

int GetMatrixSize(int number_of_vertices) {
    return number_of_vertices * (number_of_vertices - 1) / 2;
}

int SumArithmeticProgression(int num) {
    return ((double)(1 + num) / 2) * num;
}

int GetStartingPosition(int from, int number_of_vertices) {
    return (from - 1) * number_of_vertices - SumArithmeticProgression(from - 1);
}

int GetPositionInMatrix(int from, int to, int number_of_vertices) {
    if (!(from - to)) {
        return -1;
    }
    // Sorting the input to find its position in the matrix.
    int new_from = from > to ? to - 1: from - 1; 
    int new_to = from < to ? to - 1: from - 1;
    if (!new_from) {
        return new_to - 1;
    }
    return GetStartingPosition(new_from + 1, number_of_vertices) + (new_to - new_from - 1);
}

// If correct prints nothing.
bool PrintCheckInput(int N, int M) { 
    if (N < 0 || N > 5000) {
        printf("bad number of vertices");
        return false;
    }
    if (M < 0 || M > (N * (N + 1) / 2)) {
        printf("bad number of edges");
        return false;
    }
    return true;
}

bool ScanAndProcessGraph(int N, int M, int* matrix) {
    for (int i = 0; i < M; i++) {
        unsigned int weigth;
        int begin, end;
        if (scanf("%d %d %u", &begin, &end, &weigth) < 3) {
            printf("bad number of lines");
            return false;
        }
        if (begin < 1 || begin > N || end < 1 || end > N) {
            printf("bad vertex");
            return false;
        }
        if (weigth > INT_MAX) { 
            printf("bad length");
            return false;
        }
        if (begin == end) {
            return true;
        }
        matrix[GetPositionInMatrix(begin, end, N)] = weigth;
    }
    return true;
}

int GetMinPathIndex(ull* distance, bool* is_visited, int number_of_vertices) {
    ull min = LLONG_MAX;
    int min_index = 0;
    for (int i = 0; i < number_of_vertices; i++) {
        if (!is_visited[i] && distance[i] <= min) {
            min = distance[i];
            min_index = i;
        }
    }
    return min_index;
}

void PrintShortestPath(int* prev, ull* distance, int number_of_vertices, int start, int destination, char overflow_flag) {
    int* resullt = calloc(sizeof(int), number_of_vertices + 1);
    int pos = 0;
    resullt[pos] = destination;
    pos++;
    int v = destination - 1;
    bool does_path_exist = true;
    while (prev[v] != start) {
        if (pos == number_of_vertices - 1) {
            does_path_exist = false;
            break;
        }
        if (!prev[v]) {
            break;
        }
        resullt[pos] = prev[v];
        pos++;
        v = prev[v] - 1;
    }
    if (start != destination) {
        resullt[pos] = start;
    }
    pos = 0;
    printf("\n");
    if (does_path_exist && distance[destination - 1] > INT_MAX && overflow_flag >= 2) {
        printf("overflow");
    }
    else if (does_path_exist) {
        while(resullt[pos] != 0) {
            printf("%d ", resullt[pos]);
            pos++;
        }
    }
    else {
        printf("no path");
    }
    free(resullt);
}

void PrintDijkstraAlgorithm(int* matrix, int number_of_vertices, int start, int destination) {
    int* prev = calloc(sizeof(int), number_of_vertices);
    ull* distance = malloc(sizeof(ull) * number_of_vertices);
    bool* is_visited = malloc(sizeof(bool) * number_of_vertices);
    for (int i = 0; i < number_of_vertices; i++) {
        distance[i] = LLONG_MAX;
        is_visited[i] = false;
    }
    distance[start - 1] = 0;
    prev[start - 1] = start;
    char overflow_flag = 0;
    for (int i = 0; i < number_of_vertices - 1; i++) {
        int min_index = GetMinPathIndex(distance, is_visited, number_of_vertices);
        is_visited[min_index] = true;
        for (int i = 0; i < number_of_vertices; i++) {
            if (min_index == i) {
                continue;
            }
            const ull weigth = matrix[GetPositionInMatrix(min_index + 1, i + 1, number_of_vertices)];
            if (!is_visited[i] && weigth && (distance[min_index] + weigth <= distance[i])) {
                distance[i] = distance[min_index] + weigth;
                if (distance[i] > INT_MAX) {
                    overflow_flag++;
                }   
                prev[i] = min_index + 1;
            }
        }
    }
    for (int i = 0; i < number_of_vertices; i++) {
        if (distance[i] == LLONG_MAX) {
            printf("oo ");
        }
        else if (distance[i] > INT_MAX) {
            printf("INT_MAX+ ");
        }
        else {
            printf("%llu ", distance[i]);
        }
    }
    PrintShortestPath(prev, distance, number_of_vertices, start, destination, overflow_flag);
    free(prev);
    free(is_visited);
    free(distance);
}

int main(void) {
    int number_of_vertices;
    if (!scanf("%d", &number_of_vertices)) {
        return 0;
    }
    int start, destination;
    if (!scanf("%d %d", &start, &destination)) {
        return 0;
    }
    if (start < 1 || start > number_of_vertices || destination < 1 || destination > number_of_vertices) {
        printf("bad vertex");
        return 0;
    }
    int number_of_edges;
    if (!scanf("%d", &number_of_edges)) {
        return 0;
    }
    if (!PrintCheckInput(number_of_vertices, number_of_edges)) {
        return 0;
    }
    const int matrix_length = GetMatrixSize(number_of_vertices);
    int* matrix = malloc(sizeof(int) * matrix_length);
    for (int i = 0; i < matrix_length; i++) {
        matrix[i] = 0;
    }
    if (!ScanAndProcessGraph(number_of_vertices, number_of_edges, matrix)) {
        free(matrix);
        return 0;
    }
    PrintDijkstraAlgorithm(matrix, number_of_vertices, start, destination);
    free(matrix);
    return EXIT_SUCCESS;
}
