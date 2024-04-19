#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define kUcharMax 256
#define kPatternLen 16

typedef unsigned char u_char;

typedef struct {
    u_char pattern_data[kPatternLen + 1];
    int StopTable[kUcharMax];
} Pattern;

typedef struct {
    u_char text[kPatternLen + 1];
    size_t index;
    u_char text_back;
} Window;

void BuildStopTable(Pattern* pattern) {
    for (size_t i = 0; i < kUcharMax; i++) {
        pattern -> StopTable[i] = -1;
    }
    for (size_t i = 0; i < strlen((char*)pattern -> pattern_data) - 1; i++) {
        pattern -> StopTable[(int)pattern -> pattern_data[i]] = i;
    }
}

Window* BufferRealloc(Window* window, u_char* adding, size_t pattern_len, size_t shift) {
    for (size_t i = shift; i < pattern_len; i++) {
        window -> text[i - shift] =  window -> text[i];
        window -> text[i] = '\0';
    }
    int count = 0;
    for (size_t i = pattern_len - shift; i < pattern_len; i++) {
        window -> text[i] = adding[count];
        ++count;
    }
    return window;
}

int GetTimesLastLetter(u_char* pattern, size_t pattern_len) {
    int times = 0;
    for (size_t i = 0; i < pattern_len; i++) {
        if (pattern[i] == pattern[pattern_len - 1]) {
            times++;
        }
    }
    return times;
}

int BoyerMooreAlgorythmUtils (Pattern* pattern, size_t pattern_len, int times_last_letter, Window* window, FILE* output, int* global_counter) {
    bool is_compared = false;
    for (int i = pattern_len - 1; i >= 0; --i) {
        fprintf(output, "%d ", *global_counter + i + 1);
        if (pattern -> pattern_data[i] != window -> text[i]) {
            if (pattern -> StopTable[(int)window -> text[i]] != -1) {
                if (!is_compared && window -> text[i] != pattern -> pattern_data[pattern_len - 1]) {
                    return i - pattern -> StopTable[(int)window -> text[i]];
                }
                else if (window -> text[i] == pattern -> pattern_data[pattern_len - 1] && times_last_letter >= 2) {
                    return pattern_len - 1 - pattern -> StopTable[(int)pattern -> pattern_data[pattern_len - 1]];
                }
                else if (window -> text[i] == pattern -> pattern_data[pattern_len - 1] && times_last_letter == 1) {
                    return pattern_len;
                }
            }
            else {
                if (is_compared) {
                    return pattern_len - 1 - pattern -> StopTable[(int)pattern -> pattern_data[pattern_len - 1]];
                }
                else {
                    return pattern_len;
                }
            }  
        }
        else {
            is_compared = true;
        }  
    }
    return pattern_len - 1 - pattern -> StopTable[(int)pattern -> pattern_data[pattern_len - 1]];
}

void InputPattern(Pattern* pattern, FILE* input) {
    char temp;
    int last_input = 0;
    for (size_t i = 0; i < kPatternLen + 1; i++) {
        if (fread(&temp, sizeof(u_char), 1, input)) {
            if (temp == '\n') {
                break;
            }
            pattern -> pattern_data[last_input] = (u_char)temp;
            ++last_input;
        }
    }
}

int main(void) {
    FILE* input = fopen("in.txt", "r");
    FILE* output = fopen("out.txt", "a+");
    Pattern pattern;
    memset(pattern.pattern_data, '\0', kPatternLen + 1);
    Window window;
    memset(window.text, '\0', kPatternLen + 1);
    InputPattern(&pattern, input);
    BuildStopTable(&pattern);

    size_t pattern_len = strlen((char*)pattern.pattern_data);
    int amount_of_input = pattern_len;
    u_char* adding = calloc(pattern_len, sizeof(u_char));
    memset(adding, 0, pattern_len);
    int shift = pattern_len;
    int global_counter = 0;
    int times_last_letter = GetTimesLastLetter(pattern.pattern_data, pattern_len);
    while(fread(adding, sizeof(u_char), amount_of_input, input) > 0) {
        BufferRealloc(&window, adding, pattern_len, shift);
        if (strlen((char*)window.text) < pattern_len) {
            free(adding);
            fclose(input);
            fclose(output);
            return 0;
        }
        shift = BoyerMooreAlgorythmUtils(&pattern, pattern_len, times_last_letter, &window, output, &global_counter);
        global_counter += shift;
        amount_of_input = shift;
        memset(adding, 0, pattern_len);
    }

    free(adding);
    fclose(input);
    fclose(output);
    return 0;
}