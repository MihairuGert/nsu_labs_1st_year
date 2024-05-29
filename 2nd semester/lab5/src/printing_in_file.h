#pragma once

#include "bit_operations.h"
#include "huffman_tree.h"

void SaveTreeInFile(FILE* out, Node* root);
void FPrintCompressedTextInFile(FILE* in, FILE* out, Code* code_array, int8_t* trash_bits);
void FPrintTrashBitsAmount(FILE* out, int8_t trash_bits);
