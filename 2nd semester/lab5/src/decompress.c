#include "decompress.h"

#define BITS_IN_BYTE 8
#define FIRST_BIT 128
#define ALPHABET_SIZE 256
#define NODE_MARKER 300

int8_t GetTrashBitsAmount(FILE* in) {
    int8_t trash_bits_amount = 0;
    u_char carret = FIRST_BIT;
    u_char buffer;
    bool is_eof;
    if (!fread(&buffer, sizeof(char), 1, in)) {
        return 0;
    }
    for (int i = 0; i < BITS_IN_BYTE - 1; i++) {
        if (CheckBitSequencely(&buffer, &carret, NULL, in, &is_eof)) {
            SetBitInNumber(&trash_bits_amount, i);
        }
    }
    if (buffer & 1)
        trash_bits_amount++;
    return trash_bits_amount;
}

void FPrintDecompressedTextInFile(FILE* out, FILE* in, Node* huffman_tree, size_t amount_of_letters) {
    u_char carret = FIRST_BIT;
    u_char buffer, buffer_next;
    if (!amount_of_letters) {
        amount_of_letters = 8;
    }
    if (!fread(&buffer, sizeof(char), 1, in)) {
        return;
    }
    bool is_last = false;
    if (fread(&buffer_next, sizeof(char), 1, in) == 0) {
        is_last = true;
    }
    bool is_node = huffman_tree->data.letter_code == NODE_MARKER;
    Node* tree_root = huffman_tree;
    Node* current_node;
    current_node = tree_root;
    bool is_eof = false;
    while (true) {
        short bit_value;
        if (!is_last) {
            bit_value = CheckBitSequencely(&buffer, &carret, &buffer_next, in, &is_eof);
        } else {
            bit_value = CheckBitSequencelyWithCounter(&buffer, &carret, &amount_of_letters);
        }
        if (bit_value == 1 && is_node) {
            current_node = current_node->right;
            if (current_node->data.letter_code != NODE_MARKER) {
                fprintf(out, "%c", current_node->data.letter_code);
                current_node = tree_root;
            }
        } else if (bit_value == 0 && is_node) {
            current_node = current_node->left;
            if (current_node->data.letter_code != NODE_MARKER) {
                fprintf(out, "%c", current_node->data.letter_code);
                current_node = tree_root;
            }
        } else if (bit_value == 1 && !is_node) {
            fprintf(out, "%c", current_node->data.letter_code);
        }
        if (is_eof) {
            is_last = true;
        }
        if (bit_value == -1) {
            break;
        }
    }
}

void Decompress(FILE* in, FILE* out) {
    Node* huffman_tree = malloc(sizeof(Node) * (2 * ALPHABET_SIZE - 1));
    size_t trash_bits_amount = GetTrashBitsAmount(in);
    Node* huffman_tree_root = GetTreeFromFile(in, huffman_tree);
    if (!huffman_tree_root) {
        free(huffman_tree);
        return;
    }
    FPrintDecompressedTextInFile(out, in, huffman_tree_root, trash_bits_amount);
    free(huffman_tree);
}
