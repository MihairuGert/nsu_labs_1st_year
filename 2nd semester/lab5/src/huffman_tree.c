#include "huffman_tree.h"

#define INT_MAX 2147483647

int Min(int a, int b, int c) {
    if (a <= b && a <= c) {
        return 1;
    }
    if (b <= a && b <= c) {
        return 2;
    }
    return 3;
}

Node* ConnectQueuesAndTree(Node* alhabet_queue, Node* sum_queue, int size) {
    // Alphabet index.
    int alphabet_index = 0;
    int sum_index = 0;
    // Sum index used only for insertion in queue.
    int sum_insert_index = 0;
    int alphabet_flag[2] = {0, 1};
    int sum_flag[2] = {0, 1};
    int alhabet_sum_flag[2] = {0, 1};
    while (true) {
        if (alphabet_index >= size - 1) {
            alphabet_flag[0] = 1;
            alphabet_flag[1] = 0;
        }
        if (alphabet_index >= size) {
            alhabet_sum_flag[0] = 1;
            alhabet_sum_flag[1] = 0;
        }
        if (sum_index >= size - 1) {
            sum_flag[0] = 1;
            sum_flag[1] = 0;
        }
        if (alphabet_index >= size - 1 && sum_index >= size - 2) {
            break;
        }
        const int64_t alphabet_pair =
            alphabet_flag[0] * INT_MAX +
            alphabet_flag[1] *
                (alhabet_queue[alphabet_index].data.frequency +
                 alhabet_queue[alphabet_index + 1].data.frequency);
        const int64_t alphabet_and_sum_pair =
            alhabet_sum_flag[0] * (INT_MAX / 2) +
            alhabet_sum_flag[1] *
                (alhabet_queue[alphabet_index].data.frequency +
                 sum_queue[sum_index].data.frequency);
        const int64_t sum_pair =
            sum_flag[0] * INT_MAX +
            sum_flag[1] * (sum_queue[sum_index].data.frequency +
                           sum_queue[sum_index + 1].data.frequency);
        switch (Min(alphabet_pair, alphabet_and_sum_pair, sum_pair)) {
            case 1:
                sum_queue[sum_insert_index].left =
                    &alhabet_queue[alphabet_index];
                sum_queue[sum_insert_index].right =
                    &alhabet_queue[alphabet_index + 1];
                sum_queue[sum_insert_index].data.frequency =
                    alhabet_queue[alphabet_index + 1].data.frequency +
                    alhabet_queue[alphabet_index].data.frequency;
                alphabet_index += 2;
                sum_insert_index += 1;
                break;
            case 2:
                sum_queue[sum_insert_index].left =
                    &alhabet_queue[alphabet_index];
                sum_queue[sum_insert_index].right = &sum_queue[sum_index];
                sum_queue[sum_insert_index].data.frequency =
                    sum_queue[sum_index].data.frequency +
                    alhabet_queue[alphabet_index].data.frequency;
                alphabet_index += 1;
                sum_index += 1;
                sum_insert_index += 1;
                break;
            case 3:
                sum_queue[sum_insert_index].left = &sum_queue[sum_index];
                sum_queue[sum_insert_index].right = &sum_queue[sum_index + 1];
                sum_queue[sum_insert_index].data.frequency =
                    sum_queue[sum_index + 1].data.frequency +
                    sum_queue[sum_index].data.frequency;
                sum_index += 2;
                sum_insert_index += 1;
                break;
        }
    }
    return &sum_queue[sum_insert_index - 1];
}

Node* CreateHuffmanTree(Node* alhabet_queue, Node* sum_queue, int size) {
    if (size == 1) {
        Node* huffman_tree_root = malloc(sizeof(Node));
        huffman_tree_root->data.letter_code = alhabet_queue[0].data.letter_code;
        huffman_tree_root->left = NULL;
        huffman_tree_root->right = NULL;
        return huffman_tree_root;
    }
    Node* huffman_tree_root =
        ConnectQueuesAndTree(alhabet_queue, sum_queue, size);
    return huffman_tree_root;
}
