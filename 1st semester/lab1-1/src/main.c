#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 50
#define GLOBAL_RESULT_LENGTH 100

typedef unsigned long long ull;

typedef struct {
    int global_result_data[GLOBAL_RESULT_LENGTH];
    int index;
} GlobalResult;

void ClearString(unsigned char* string, size_t length) {
    for (size_t i = 0; i < length; i++) {
        string[i] = '\0';
    }
}

void PrintArray(int* array, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (array[i] != 0) {
            printf("%d ", array[i]);
        }
    }
}

int PowOptimised(int number, int power) {
    int res = 1;
    if (power > 0) {
        for (int i = 0; i < power; i++) {
            res *= number;
        }
    }
    return res;
}

ull GetHash(unsigned char string[], size_t length) {
    ull result = 0;
    for (size_t i = 0; i < length; i++) {
        result = result + (((int)string[i] % 3) * PowOptimised(3, i));
    }
    return result;
}

long CalculateMaxPower(size_t sample_length) {
    long max_power = 1;
    for (size_t i = 1; i < sample_length; i++) {
        max_power *= 3;
    }
    return max_power;
}

ull HashShift(ull hash, unsigned char element_to_add, unsigned char element_to_delete, long max_power) {
    ull result = hash;
    result = ((result - ((int)element_to_delete % 3)) / 3) + ((int)element_to_add % 3) * max_power;
    return result;
}

void FindEntranceRobinKarp(unsigned char text[], unsigned char* sample, ull sample_hash, GlobalResult* global_result, int* overall_count_saver) {
    size_t text_length = strlen((char*)text);
    size_t sample_length = strlen((char*)sample);
    unsigned char* starting_text = malloc(sample_length * sizeof(char));
    for (size_t i = 0; i < sample_length; i++) {
        starting_text[i] = text[i];
    }
    ull text_hash = GetHash(starting_text, sample_length);
    for (size_t i = 0; i < text_length - sample_length + 1; i++) {
        if (text_hash == sample_hash) {
            int index = 0;
            for (size_t j = i; j < i + sample_length; j++) {
                global_result->global_result_data[global_result->index] = *overall_count_saver + j + 1;
                global_result->index++;
                if (text[j] != sample[index]) {
                    break;
                }
                index++;
            }
        }
        if (i < text_length - sample_length) {
            text_hash = HashShift(text_hash, text[i + sample_length], text[i], CalculateMaxPower(sample_length));
        }
    }
    free(starting_text);
}

int main(void) {
    unsigned char* buffer = calloc(BUFFER_LENGTH, sizeof(char));
    ClearString(buffer, BUFFER_LENGTH);
    GlobalResult global_result;
    global_result.index = 0;
    for (size_t i = 0; i < GLOBAL_RESULT_LENGTH; i++) {
        global_result.global_result_data[i] = 0;
    }
    
    int overall_count = 0, count = 0;
    unsigned char sample[17] = {0};
    char temp;

    while (scanf("%c", &temp)) {
        if (temp == '\n') break;
        for (int i = 0; i < 16; i++) {
            if (sample[i] == '\0') {
                sample[i] = temp;
                break;
            }
        }
    }

    ull sample_hash = GetHash(sample, strlen((char*)sample));
    int input_text = 0;
    int overall_count_saver = 0;

    while ((input_text = fgetc(stdin)) != EOF) {
        if (input_text == '\n') {
            count = 0;
            FindEntranceRobinKarp(buffer, sample, sample_hash, &global_result, &overall_count_saver);
            overall_count += 1;
            overall_count_saver = overall_count;
            ClearString(buffer, BUFFER_LENGTH);
        } else {
            buffer[count] = (unsigned char)input_text;
            count++;
            overall_count++;
        }
    }
    FindEntranceRobinKarp(buffer, sample, sample_hash, &global_result, &overall_count_saver);
    printf("%d ", (int)sample_hash);
    PrintArray(global_result.global_result_data, GLOBAL_RESULT_LENGTH);
    free(buffer);
    return EXIT_SUCCESS;
}
