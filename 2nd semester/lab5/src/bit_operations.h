#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_CODE_LENGTH 256

typedef unsigned char u_char;

typedef struct Letter {
    int frequency;
    short letter_code;
} Letter;

typedef struct Node {
    struct Node* right;
    struct Node* left;
    Letter data;
} Node;

typedef struct Code {
    char code[MAX_CODE_LENGTH];
} Code;

bool AddBit(FILE* out, u_char* num, u_char* carret, int8_t what_to_add, bool are_letters_in_buffer, int8_t* used_bits);
bool CheckBitInLetter(u_char letter, int8_t bit);
bool Print8BitLetter(u_char* buffer, u_char letter, u_char* carret, FILE* out, bool are_letters_in_buffer);
void SetBitInNumber(int8_t* number, int8_t bit);
short CheckBitSequencelyWithCounter(u_char* num, u_char* carret, size_t* counter);
short CheckBitSequencely(u_char* num, u_char* carret, u_char* buffer2, FILE* in, bool* is_eof);
