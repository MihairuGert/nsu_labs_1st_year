#include <stdlib.h>
#include <stdio.h>

void Swap(int* element1, int* element2) {
    int c = *element1;
    *element1 = *element2;
    *element2 = c;
}

void QuickSort(int* input_array, int size) {
    int size_buffer = size - 1;
    int central_element = input_array[size_buffer / 2];
    int counter = 0;
    int temporary = 0;
    do {
        while (input_array[counter] < central_element) {
            ++counter;
        }
        while (input_array[size_buffer] > central_element) {
            --size_buffer;
        }

        if (counter <= size_buffer) {
            Swap(input_array[size_buffer], input_array[counter]);
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

int main(void) {
    int array_size = 0;
    if (scanf("%d", &array_size) == 0) {
        return 0;
    }
    if (array_size == 0) {
        return EXIT_SUCCESS;
    }
    int *input_array = calloc(array_size, sizeof(int));
    for (int i = 0; i < array_size; i++) {
        if (scanf("%d", &input_array[i]) == 0) {
            return 0;
        }
    }
    QuickSort(input_array, array_size);
    for (int i = 0; i < array_size; i++) {
        printf("%d ", input_array[i]);
    }
    free(input_array);
    return EXIT_SUCCESS;
}

