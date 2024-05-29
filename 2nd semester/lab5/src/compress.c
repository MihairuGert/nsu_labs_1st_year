#include "compress.h"

#define MAX_CODE_LENGTH 256
#define ALPHABET_SIZE 256
#define NODE_MARKER 300
#define INT_MAX 2147483647

typedef enum Command {
    SAVE_SPACE_FOR_BITS = 0,
    WRITE_BITS_IN_SAVED_SPACE = 1,
} Command;

int CompareLettersFrequency(const void* a, const void* b) {
    return (*(Letter*)b).frequency - (*(Letter*)a).frequency;
}

int CountUniqueLetters(Letter* alphabet) {
    int count = 0;
    while (alphabet[count].frequency != 0) {
        count++;
        if (count + 1 == ALPHABET_SIZE) {
            return count + 1;
        }
    }
    return count;
}

void FillAlphabetWithLettersFrequency(FILE* in, Letter* alphabet, size_t* amount_of_letters) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        alphabet[i].letter_code = (u_char)i;
        alphabet[i].frequency = 0;
    }
    u_char letter;
    bool is_fread_succeed = true;
    while (is_fread_succeed) {
        is_fread_succeed = fread(&letter, sizeof(char), 1, in);
        if (!is_fread_succeed) {
            return;
        }
        alphabet[letter].frequency++;
        (*amount_of_letters)++;
    }
}

Node* CreateAlphabetQueue(Letter* alphabet, int size) {
    Node* queue = malloc(sizeof(Node) * (size + 2));
    int count = 0;
    for (int i = size - 1; i >= 0; i--) {
        queue[count].data = alphabet[i];
        queue[count].left = NULL;
        queue[count].right = NULL;
        count++;
    }
    queue[size].data.frequency = 0;
    queue[size + 1].data.frequency = 0;
    return queue;
}

Node* CreateSumQueue(int size) {
    Node* sum_queue = malloc(sizeof(Node) * (size + 2));
    for (int i = 0; i < size; i++) {
        sum_queue[i].data.frequency = INT_MAX / 2;
        sum_queue[i].data.letter_code = NODE_MARKER;
        sum_queue[i].left = NULL;
        sum_queue[i].right = NULL;
    }
    sum_queue[size].data.frequency = 0;
    sum_queue[size + 1].data.frequency = 0;
    return sum_queue;
}

Node* GetAlphabetQueue(FILE* in, int* size, size_t* amount_of_letters) {
    Letter alphabet[ALPHABET_SIZE];
    FillAlphabetWithLettersFrequency(in, alphabet, amount_of_letters);
    if (!amount_of_letters) {
        return NULL;
    }
    if (!(*amount_of_letters)) {
        return NULL;
    }
    qsort(alphabet, ALPHABET_SIZE, sizeof(Letter), CompareLettersFrequency);
    *size = CountUniqueLetters(alphabet);
    Node* alphabet_queue = CreateAlphabetQueue(alphabet, *size);
    return alphabet_queue;
}

void FillCodeArrayWithCodes(Code* code_array, Node* huffman_tree_root, u_char code[], int index) {
    if (!huffman_tree_root) {
        return;
    }
    if (huffman_tree_root->data.letter_code != NODE_MARKER) {
        memmove(code_array[huffman_tree_root->data.letter_code].code, code, MAX_CODE_LENGTH);
        return;
    }
    u_char code_copy[MAX_CODE_LENGTH];
    memmove(code_copy, code, MAX_CODE_LENGTH);
    code[index] = '0';
    FillCodeArrayWithCodes(code_array, huffman_tree_root->left, code, index + 1);
    code_copy[index] = '1';
    FillCodeArrayWithCodes(code_array, huffman_tree_root->right, code_copy, index + 1);
}

Code* GetCodesArray(Node* huffman_tree, int size) {
    Code* code_array = malloc(sizeof(Code) * ALPHABET_SIZE);
    u_char temp_code[MAX_CODE_LENGTH] = {0};
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (int j = 0; j < MAX_CODE_LENGTH; j++) {
            code_array[i].code[j] = '\0';
        }
    }
    FillCodeArrayWithCodes(code_array, huffman_tree, temp_code, 0);
    if (size == 1)
        code_array[huffman_tree->data.letter_code].code[0] = '1';
    return code_array;
}

void SaveTrashBitsAmount(FILE* out, int8_t trash_bits_amount, Command command) {
    switch (command) {
        case SAVE_SPACE_FOR_BITS:
            fprintf(out, "%c", '0');
            break;
        case WRITE_BITS_IN_SAVED_SPACE:
            fseek(out, 0, SEEK_SET);
            FPrintTrashBitsAmount(out, trash_bits_amount);
            break;
    }
}

void FreeHuffmanTree(Node* sum_queue, Node* alphabet_queue, Node* huffman_tree_root, int size) {
    // If the amount of letters is 1, huffman_tree was not created using queues.
    if (size == 1) {
        free(huffman_tree_root);
    }
    free(sum_queue);
    free(alphabet_queue);
}

void Compress(FILE* in, FILE* out) {
    int size = 0;
    size_t amount_of_letters = 0;
    Node* alphabet_queue = GetAlphabetQueue(in, &size, &amount_of_letters);
    // If there is not any input, there is not any output.
    if (!alphabet_queue) {
        free(alphabet_queue);
        return;
    }
    Node* sum_queue = CreateSumQueue(size);
    Node* huffman_tree_root = CreateHuffmanTree(alphabet_queue, sum_queue, size);
    SaveTrashBitsAmount(out, 0, SAVE_SPACE_FOR_BITS);
    SaveTreeInFile(out, huffman_tree_root);
    Code* code_array = GetCodesArray(huffman_tree_root, size);
    int8_t trash_bits_amount = 0;
    FPrintCompressedTextInFile(in, out, code_array, &trash_bits_amount);
    SaveTrashBitsAmount(out, trash_bits_amount, WRITE_BITS_IN_SAVED_SPACE);
    FreeHuffmanTree(sum_queue, alphabet_queue, huffman_tree_root, size);
    free(code_array);
}
