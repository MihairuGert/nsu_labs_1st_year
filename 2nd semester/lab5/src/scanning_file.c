#include "scanning_file.h"

#define BITS_IN_BYTE 8
#define FIRST_BIT 128
#define NODE_MARKER 300

u_char GetLetter(FILE* in, u_char* carret, u_char* buffer) {
    u_char letter = 0;
    u_char letter_carret = FIRST_BIT;
    bool is_eof = false;
    for (int i = 0; i < BITS_IN_BYTE; i++) {
        if (CheckBitSequencely(buffer, carret, NULL, in, &is_eof)) {
            letter = letter | letter_carret;
            letter_carret = letter_carret >> 1;
        } else {
            letter_carret = letter_carret >> 1;
        }
    }
    return letter;
}

Node* GetTreeFromFileInternal(FILE* in, u_char* carret, u_char* buffer, Node* huffman_tree, int* index, bool* is_eof) {
    int index_copy = *index;
    if (CheckBitSequencely(buffer, carret, NULL, in, is_eof)) {
        huffman_tree[*index].data.letter_code = NODE_MARKER;
        (*index)++;
        huffman_tree[index_copy].left = GetTreeFromFileInternal(in, carret, buffer, huffman_tree, index, is_eof);
        huffman_tree[index_copy].right = GetTreeFromFileInternal(in, carret, buffer, huffman_tree, index, is_eof);
    } else {
        u_char letter = GetLetter(in, carret, buffer);
        huffman_tree[*index].data.letter_code = letter;
        huffman_tree[*index].right = NULL;
        huffman_tree[*index].left = NULL;
        Node* node_ptr = &huffman_tree[*index];
        (*index)++;
        return node_ptr;
    }
    return &huffman_tree[index_copy];
}

Node* GetTreeFromFile(FILE* in, Node* huffman_tree) {
    int index = 0;
    u_char carret = FIRST_BIT;
    u_char buffer;
    if (!fread(&buffer, sizeof(char), 1, in)) {
        return NULL;
    }
    bool is_eof = false;
    Node* huffman_tree_root = GetTreeFromFileInternal(in, &carret, &buffer, huffman_tree, &index, &is_eof);
    return huffman_tree_root;
}
