#include "printing_in_file.h"

#define BITS_IN_BYTE 8
#define FIRST_BIT 128
#define NODE_MARKER 300

void PrintNodeInFile(FILE* out,
                     Node* root,
                     u_char* buffer,
                     u_char* carret,
                     bool* are_letters_in_buffer) {
    if (root->data.letter_code == NODE_MARKER) {
        *are_letters_in_buffer = true;
        *are_letters_in_buffer = AddBit(out, buffer, carret, 1, *are_letters_in_buffer, NULL);
    } else {
        *are_letters_in_buffer = true;
        *are_letters_in_buffer = AddBit(out, buffer, carret, 0, *are_letters_in_buffer, NULL);
        *are_letters_in_buffer = Print8BitLetter(buffer, root->data.letter_code, carret, out, *are_letters_in_buffer);
    }
}

void PrintTreeByReverseTraversal(FILE* out,
                               Node* root,
                               u_char* buffer,
                               u_char* carret,
                               bool* are_letters_in_buffer) {
    if (root == NULL) {
        return;
    }
    PrintNodeInFile(out, root, buffer, carret, are_letters_in_buffer);
    PrintTreeByReverseTraversal(out, root->left, buffer, carret, are_letters_in_buffer);
    PrintTreeByReverseTraversal(out, root->right, buffer, carret, are_letters_in_buffer);
}

void SaveTreeInFile(FILE* out, Node* root) {
    u_char carret = FIRST_BIT;
    u_char buffer = 0;
    bool are_letters_in_buffer = false;
    PrintTreeByReverseTraversal(out, root, &buffer, &carret, &are_letters_in_buffer);
    if (are_letters_in_buffer)
        fprintf(out, "%c", buffer);
}

void FPrintCompressedLetter(FILE* out,
                            u_char letter,
                            Code* code_array,
                            u_char* buffer,
                            u_char* carret,
                            bool* are_letters_in_buffer,
                            int8_t* trash_bits) {
    int index = 0;
    while (code_array[letter].code[index] != '\0') {
        *are_letters_in_buffer = true;
        if (code_array[letter].code[index] == '0') {
            *are_letters_in_buffer =
                AddBit(out, buffer, carret, 0, *are_letters_in_buffer, trash_bits);
        } else if (code_array[letter].code[index] == '1') {
            *are_letters_in_buffer =
                AddBit(out, buffer, carret, 1, *are_letters_in_buffer, trash_bits);
        }
        index++;
    }
}

void FPrintCompressedTextInFile(FILE* in,
                                FILE* out,
                                Code* code_array,
                                int8_t* trash_bits) {
    fseek(in, 1, SEEK_SET);
    u_char temp;
    u_char carret = FIRST_BIT;
    u_char buffer = 0;
    bool are_letters_in_buffer = false;
    while (fread(&temp, sizeof(char), 1, in)) {
        FPrintCompressedLetter(out, temp, code_array, &buffer, &carret,
                               &are_letters_in_buffer, trash_bits);
    }
    if (are_letters_in_buffer) {
        fprintf(out, "%c", buffer);
    }
}

void FPrintTrashBitsAmount(FILE* out, int8_t trash_bits) {
    u_char carret = FIRST_BIT;
    u_char buffer = 0;
    bool are_letters_in_buffer = false;
    for (int i = 0; i < BITS_IN_BYTE; i++) {
        if (CheckBitInLetter(trash_bits, i)) {
            are_letters_in_buffer = AddBit(out, &buffer, &carret, 1, are_letters_in_buffer, NULL);
        } else {
            are_letters_in_buffer = AddBit(out, &buffer, &carret, 0, are_letters_in_buffer, NULL);
        }
    }
}
