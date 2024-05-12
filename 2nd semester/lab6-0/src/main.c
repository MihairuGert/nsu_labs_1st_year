#include "avl_tree.h"

int main(void) {
    int amount_of_nodes;
    if (!scanf("%d", &amount_of_nodes)) {
        return 0;
    }
    if (amount_of_nodes == 0) {
        printf("0");
        return 0;
    }
    Node* node_array = malloc(sizeof(Node) * amount_of_nodes);  // Array, where every node is stored.
    int index = 0;                                              // Index of node in Node array.
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
