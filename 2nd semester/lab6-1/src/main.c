#include <stdio.h>
#include <stdlib.h>

typedef enum Color {
    BLACK = 0,
    RED = 1,
} Color;

typedef enum NodeType {
    CHILD = 3,
    PARENT = 2,
    GRANDPARENT = 1,
    NO_TYPE = 0,
} NodeType;

typedef struct Node {
    struct Node* right;
    struct Node* left;
    int key;
    Color color;
} Node;

Node* InitializeNode(Node* node_array, int index, int key) {
    node_array[index].color = RED;
    node_array[index].key = key;
    node_array[index].right = NULL;
    node_array[index].left = NULL;
    return &node_array[index];
}

Node* GetUncle(Node* parent, Node* grandparent) {
    if (!grandparent || !parent) {
        return NULL;
    }
    return parent == grandparent->left ? grandparent->right : grandparent->left;
}

int GetBlackHeight(Node* root) { 
    if (!root) {
        return 1;
    }
    if (root->color == BLACK) {
        return GetBlackHeight(root->right) + 1;
    }
    else if (root->color == RED) {
        return GetBlackHeight(root->right);
    }
    return 1;
}

void SwapNodesKeysAndColor(Node* a, Node* b) {
    int keyb = b->key;
    char colorb = b->color;
    b->key = a->key;
    a->key = keyb;
    b->color = a->color;
    a->color = colorb;
}

void RotateRight(Node* node) {
    SwapNodesKeysAndColor(node, node->left);
    Node* buffer = node->right;
    node->right = node->left;
    node->left = node->right->left;
    node->right->left = node->right->right;
    node->right->right = buffer;
}

void RotateLeft(Node* node) {
    SwapNodesKeysAndColor(node, node->right);
    Node* buffer = node->left;
    node->left = node->right;
    node->right = node->left->right;
    node->left->right = node->left->left;
    node->left->left = buffer;
}

void Balance(Node* node, Node* parent, Node* grandparent, NodeType* prev) {
    if (!node) {
        return;
    }
    else if (!parent) {
        node->color = BLACK;
        return;
    }
    else if (parent->color == BLACK) {
        return;
    }
    else if (!grandparent) {
        parent->color = BLACK;
        return;
    }
    Node* uncle = GetUncle(parent, grandparent);
    if (uncle && uncle->color == RED) {
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        *prev = CHILD;
    }
    else if (uncle == grandparent->right) {
        if (node == parent->left) {
            parent->color = BLACK;
            grandparent->color = RED;
            RotateRight(grandparent);
        }
        else if (node == parent->right) {
            RotateLeft(parent);
            parent->color = BLACK;
            grandparent->color = RED;
            RotateRight(grandparent);
        }
    }
    else if (uncle == grandparent->left) {
        if (node == parent->right) {
            parent->color = BLACK;
            grandparent->color = RED;
            RotateLeft(grandparent);
        }
        else if (node == parent->left) {
            RotateRight(parent);
            parent->color = BLACK;
            grandparent->color = RED;
            RotateLeft(grandparent);
        }
    }
}
void InsertNode(int key, int* index, Node* main_node, Node* node_array, int nodes_amount, Node* parent, NodeType* node_type) { 
    short flag = 0;
    if ((*index) == 0) {
        InitializeNode(node_array, *index, key)->color = BLACK;
        (*index)++;
        return;
    }
    if (key < main_node->key) {
        if (main_node->left == NULL) {
            main_node->left = InitializeNode(node_array, *index, key);
            Balance(main_node->left, main_node, parent, node_type);
            (*index)++;
        }
        else {
            InsertNode(key, index, main_node->left, node_array, nodes_amount, main_node, node_type);
            flag = 1;
        }
    }
    else {
        if (main_node->right == NULL) {
            main_node->right = InitializeNode(node_array, *index, key);
            Balance(main_node->right, main_node, parent, node_type);
            (*index)++;
        }
        else {
            InsertNode(key, index, main_node->right, node_array, nodes_amount, main_node, node_type);
            flag = 2;
        }
    }
    if (*node_type == CHILD) {
        (*node_type) = PARENT;
    }
    else if (*node_type == PARENT) {
        (*node_type) = GRANDPARENT;
    }
    else if (*node_type == GRANDPARENT) {
        if (flag == 2) {
            if (main_node && parent) {
                (*node_type) = NO_TYPE;
                Balance(main_node->right, main_node, parent, node_type);
                (*node_type)--;
            }
        }
        if (flag == 1) {
            if (main_node && parent) {
                (*node_type) = NO_TYPE;
                Balance(main_node->left, main_node, parent, node_type);
                (*node_type)--;
            }
        }
    }
}

int main() {
    int nodes_amount;
    if (!scanf("%d", &nodes_amount))
        return 0;
    if (nodes_amount == 0) {
        printf("0");
        return 0;
    }
    Node* node_array = malloc(nodes_amount * sizeof(Node));
    int index = 0;
    for (int i = 0; i < nodes_amount; i++) {
        int key;
        if (!scanf("%d", &key))
            return 0;
        NodeType node_type = NO_TYPE;
        InsertNode(key, &index, &node_array[0], node_array, nodes_amount, NULL, &node_type);
        node_array[0].color = BLACK;
    }
    printf("%d", GetBlackHeight(&node_array[0]));
    free(node_array);
    return 0;
}
