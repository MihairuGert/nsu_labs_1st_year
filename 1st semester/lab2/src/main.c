#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Length of the input string.
#define INPUT_LENGTH 50
#define CHAR_TO_INT_COEF 48

void PrintString(char* string, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", string[i]);
    }  
}

void Swap(char* element1, char* element2) {
    char c = *element1;
    *element1 = *element2;
    *element2 = c;
}

int Factorial(int number) {
    int result = 1;
    for (int i = 1; i <= number; i++) {
        result *= i;
    }
    return result;
}

// Make string reversed.
void ReverseString(char* string, size_t start, size_t end) {
    if (start == end) {
        return;
    }
    size_t count = 0;
    for (size_t i = end; i > (end + start) / 2; --i) {
        Swap(&string[i], &string[start + count]);
        count++;
    }
}

// Check symbol if it is in range from 0 to 9.
int CheckInput(char* string, int length) {
    short duplications[10][1];
    for (int i = 0; i < 10; i++) {
        duplications[i][0] = 0;
    }
    for (int i = 0; i < length; i++) {
        if (string[i] < '0' || string[i] > '9') {
            return 0;
        }
        duplications[(int)string[i] - CHAR_TO_INT_COEF][0]++;
    }
    for (int i = 0; i < 10; i++) {
        if (duplications[i][0] > 1) {
            return 0;
        }
    }
    return 1;
}

void PrintNextPermutation(char* string, int length) {
    for (int i = length - 2; i >= 0; --i) {
        if (string[i] >= string[i + 1]) {
            continue;
        }
        unsigned int index_small = i;
        unsigned int index_big = 0;
        for (int j = length - 1; j > 0; --j) {
            if (string[i] < string[j]) {
                index_big = j;
                break;
            }
        }
        Swap(&string[i], &string[index_big]);
        ReverseString(string, index_small + 1, length - 1);
        PrintString(string, length);
        break;
    }
}

int main(void) {
    char input_string[INPUT_LENGTH];
    for (int i = 0; i < INPUT_LENGTH; i++) {
        input_string[i] = '\0';
    }
    char temp;
    int last_position = 0;
    while(scanf("%c", &temp)) {
        if (temp == '\n') break;
        input_string[last_position] = temp;
        last_position++;
    }
    int number_of_permutations = 0;
    if (scanf("%d", &number_of_permutations) == 0) {
        return 0;
    }

    if (CheckInput(input_string, strlen(input_string)) == 0) {
        printf("bad input");
        return 0;
    }
    long factorial = Factorial(strlen(input_string));
    for (int i = 0; i < number_of_permutations; i++) {
        if (i >= factorial - 1) {
            break;
        }
        PrintNextPermutation(input_string, strlen(input_string));
        printf("\n");
    }

    return EXIT_SUCCESS;
}
