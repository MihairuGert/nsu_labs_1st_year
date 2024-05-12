#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int height;
    struct Node* right;
    struct Node* left;
} Node;

Node* ConstructNode(int key, int* index, Node* node_array);
int Maximum(int n1, int n2);
int GetHeight(Node* node);
void HeightUpdate(Node* node);
int GetHeightDifference(Node* node);
void SwapNodesKeys(Node* a, Node* b);
void RightBalance(Node* main_node);
void LeftBalance(Node* main_node);
void Balance(Node* main_node);
void NodeInsert(Node* main_node, int key, int* index, Node* node_array);