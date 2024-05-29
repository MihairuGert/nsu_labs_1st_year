#include "bit_operations.h"

#define BITS_IN_BYTE 8
#define FIRST_BIT 128

void ResetCarret(u_char* carret) {
    *carret = FIRST_BIT;
}

bool AddBit(FILE* out, u_char* buffer, u_char* carret, int8_t what_to_add, bool are_letters_in_buffer, int8_t* trash_bits) {
    if (what_to_add == 1)
        *buffer = *buffer | *carret;
    if (trash_bits) 
        (*trash_bits)++;
    if ((*carret) >> 1 == 0) {
        ResetCarret(carret);
        fprintf(out, "%c", *buffer);
        if (trash_bits) 
            (*trash_bits) = 0;
        are_letters_in_buffer = false;
        *buffer = 0;
        return are_letters_in_buffer;
    }
    *carret = (*carret) >> 1;
    return are_letters_in_buffer;
}

bool CheckBitInLetter(u_char letter, int8_t bit) {
    const u_char mask = 1u << (BITS_IN_BYTE - bit - 1);
    return letter & mask;
}

bool Print8BitLetter(u_char* buffer, u_char letter, u_char* carret, FILE* out, bool are_letters_in_buffer) {
    for (int8_t i = 0; i < BITS_IN_BYTE; i++) {
        are_letters_in_buffer = true;
        if (CheckBitInLetter(letter, i)) {
            are_letters_in_buffer = AddBit(out, buffer, carret, 1, are_letters_in_buffer, NULL);
        } else {
            are_letters_in_buffer = AddBit(out, buffer, carret, 0, are_letters_in_buffer, NULL);
        }
    }
    return are_letters_in_buffer;
}

void SetBitInNumber(int8_t* number, int8_t bit) {
    const int8_t mask = 1 << (BITS_IN_BYTE - bit - 1);
    *number = *number | mask;
}

short CheckBitSequencelyWithCounter(u_char* num, u_char* carret, size_t* counter) {
    if (!(*counter)) {
        return -1;
    }
    short bit_value = 0;
    if ((*carret) & (*num)) {
        bit_value = 1;
    }
    (*counter)--;
    *carret = *carret >> 1;
    return bit_value;
}

short CheckBitSequencely(u_char* num, u_char* carret, u_char* buffer2, FILE* in, bool* is_eof) {
    short bit_value = 0;
    if ((*carret) & (*num)) {
        bit_value = 1;
    }
    if ((*carret) >> 1 == 0) {
        ResetCarret(carret);
        if (buffer2) {
            *num = *buffer2;
            if (!fread(buffer2, sizeof(u_char), 1, in)) {
                *is_eof = true;
            }
        }
        else {
            if (!fread(num, sizeof(u_char), 1, in))
                *is_eof = true;
        }
        return bit_value;
    }
    *carret = *carret >> 1;
    return bit_value;
}
