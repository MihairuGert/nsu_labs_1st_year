#include "avl_tree.h"

Node* ConstructNode(int key, int* index, Node* node_array) {
    node_array[*index].key = key;
    node_array[*index].left = NULL;
    node_array[*index].right = NULL;
    node_array[*index].height = 0;
    return &node_array[*index];
}

int Maximum(int n1, int n2) {
    return n1 >= n2 ? n1 : n2;
}

int GetHeight(Node* node) {
    return node == NULL ? -1 : node->height;
}

void HeightUpdate(Node* node) {
    node->height = Maximum(GetHeight(node->right), GetHeight(node->left)) + 1;
}

int GetHeightDifference(Node* node) {
    return (node == NULL) ? 0 : GetHeight(node->right) - GetHeight(node->left);
}

void SwapNodesKeys(Node* a, Node* b) {
    int temp = b->key;
    b->key = a->key;
    a->key = temp;
}

void RightBalance(Node* main_node) {
    SwapNodesKeys(main_node, main_node->left);
    Node* buffer = main_node -> right;
    main_node -> right = main_node -> left;
    main_node -> left = main_node -> right -> left;
    main_node -> right -> left = main_node -> right -> right;
    main_node -> right -> right = buffer;
    HeightUpdate(main_node -> right);
    HeightUpdate(main_node);
}

void LeftBalance(Node* main_node) {
    SwapNodesKeys(main_node, main_node->right);
    Node* buffer = main_node -> left;
    main_node -> left = main_node -> right;
    main_node -> right = main_node -> left -> right;
    main_node -> left -> right = main_node -> left -> left;
    main_node -> left -> left = buffer;
    HeightUpdate(main_node -> left);
    HeightUpdate(main_node);
}

void Balance(Node* main_node) {
    int balance = GetHeightDifference(main_node);
    if (balance == -2) {
        if (GetHeightDifference(main_node->left) == 1)
            LeftBalance(main_node->left);
        RightBalance(main_node);
    } else if (balance == 2) {
        if (GetHeightDifference(main_node->right) == -1)
            RightBalance(main_node->right);
        LeftBalance(main_node);
    }
}

void NodeInsert(Node* main_node, int key, int* index, Node* node_array) {
    if (key < main_node->key) {
        if (main_node->left == NULL) {
            main_node->left = ConstructNode(key, index, node_array);
            (*index)++;
        } else {
            NodeInsert(main_node->left, key, index, node_array);
        }
    } else {
        if (main_node->right == NULL) {
            main_node->right = ConstructNode(key, index, node_array);
            (*index)++;
        } else {
            NodeInsert(main_node->right, key, index, node_array);
        }
    }
    HeightUpdate(main_node);
    Balance(main_node);
}
