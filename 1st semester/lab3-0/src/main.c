#include <stdlib.h>
#include <stdio.h>

// Swaps elements in the memory. 
void Swap(int* num1, int* num2) {
    int c = *num1;
    *num1 = *num2;
    *num2 = c;
}
// Creating a hip from the array.
void MakeHip(int* array, int length, int root) {
    int max_element = root;
    int left_element = 2 * root + 1;
    int right_element = 2 * root + 2;
    // Define the minimal element.
    if (left_element < length && array[left_element] > array[max_element]) {
        max_element = left_element;
    }
    if (right_element < length && array[right_element] > array[max_element]) {
        max_element = right_element;
    }
    // Change the hip.
    if (max_element != root) {
        Swap(&array[max_element], &array[root]);
        MakeHip(array, length, max_element);
    }
}

int main(void) {
    int length = 0;
    if (scanf("%d", &length) == 0) {
        return 0;
    }
    int* arr = malloc(length * sizeof(int)); 
    for (int i = 0; i < length; i++) {
        if (scanf("%d", &arr[i]) == 0) {
            return 0;
        }
    }
    // Creating a hip from the array.
    for (int i = length / 2 - 1; i >= 0; --i) {
        MakeHip(arr, length, i);
    }
    // Sorting the array.
    for (int i = length - 1; i > 0; --i) {
        Swap(&arr[0], &arr[i]);
        MakeHip(arr, i, 0);
    }
    // Printing the array.
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    free(arr);
    return EXIT_SUCCESS;
}
