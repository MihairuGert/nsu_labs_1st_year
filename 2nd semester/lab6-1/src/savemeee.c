#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum Color {
    RED = 1,
    BLACK = 0,
};

typedef struct Node {
    struct Node* right;
    struct Node* parent;
    struct Node* left;
    int color;
    int key;
} Node;

void SwapNodesKeys(Node* a, Node* b) {
    int keyb = b->key;
    b->key = a->key;
    a->key = keyb;
}

Node* ConstructNode(int key, int* index, Node* node_array, Node* blank) {
    node_array[*index].key = key;
    node_array[*index].color = RED;
    node_array[*index].left = blank;
    node_array[*index].right = blank;
    node_array[*index].parent = blank;
    return &node_array[*index];
}

Node* ConstructBlank(int key, int* index, Node* node_array, Node* blank) {
    node_array[*index].key = key;
    node_array[*index].color = RED;
    node_array[*index].left = blank;
    node_array[*index].right = blank;
    node_array[*index].parent = blank;
    return &node_array[*index];
}

void RightBalance(Node* main_node) {
    SwapNodesKeys(main_node, main_node->left);
    Node* buffer = main_node->right;
    main_node->right = main_node->left;
    main_node->left = main_node->right->left;
    main_node->right->left = main_node->right->right;
    main_node->right->right = buffer;
    HeightUpdate(main_node->right);
    HeightUpdate(main_node);
}

void LeftBalance(Node* main_node) {
    SwapNodesKeys(main_node, main_node->right);
    Node* buffer = main_node->left;
    main_node->left = main_node->right;
    main_node->right = main_node->left->right;
    main_node->left->right = main_node->left->left;
    main_node->left->left = buffer;
    HeightUpdate(main_node->left);
    HeightUpdate(main_node);
}

bool IsNodeAlive(Node* node, Node* blank) {
    return node != blank;
} 

void NodeInsert(Node* main_node, int key, int* index, Node* node_array) {
    
}

void Balance(Node* node_array, Node* new_node, int* index) {
    Node* uncle;
    while (new_node->parent->color == RED) {
        if (new_node->parent == new_node->parent->parent->left) {
            uncle = new_node->parent->parent->right;
            if(uncle->color == RED) {   // 2nd and 3rd cases.
                new_node->parent->color = BLACK;
                uncle->color = BLACK;
                new_node->parent->parent->color = RED;
            }
            else {
                if (new_node->parent->color == RED) {   // 4th case.
                    LeftBalance(new_node->parent);
                }
                new_node->parent->color = BLACK;    // 5th case.
                new_node->parent->parent->color = RED;
                RightBalance(new_node->parent->parent);
            }
        }
        else {
            uncle = new_node->parent->parent_left;
            if(uncle->color == RED) {   // 2nd and 3rd cases.
                new_node->parent->color = BLACK;
                uncle->color = BLACK;
                new_node->parent->parent->color = RED;
            }
            else {
                if (new_node->parent->color == RED) {   // 4th case.
                    RightBalance(new_node->parent);
                }
                new_node->parent->color = BLACK;    // 5th case.
                new_node->parent->parent->color = RED;
                LeftBalance(new_node->parent->parent);
            }
        }
    }
    node_array[0].color = BLACK;    // 1st case.
}

int main(void) {
    int amount_of_nodes;
    if (!scanf("%d", &amount_of_nodes)) {
        return 0;
    }
    if (amount_of_nodes == 0) {
        printf("0");
        return 0;
    }
    Node* node_array = malloc(sizeof(Node) * amount_of_nodes + 1);
    int index = 0;                                              // Index of node in Node array.
    Node* blank = ConstructNode(-999, amount_of_nodes, node_array, )
    for (int i = 0; i < amount_of_nodes; i++) {
        int key;
        if (!scanf("%d", &key)) {
            return 0;
        }
        if (i == 0) {
            ConstructNode(key, &index, node_array);
            index++;
        } else {
            NodeInsert(&node_array[0], key, &index, node_array);
        }
    }
    printf("%d", node_array[0].height + 1);
    free(node_array);
    return EXIT_SUCCESS;
}
