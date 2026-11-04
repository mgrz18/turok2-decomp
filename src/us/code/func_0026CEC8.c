#include "common.h"

/* A node of a tree whose nodes have up to three children. */
typedef struct Node {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Node *child0;
    /* 0x14 */ struct Node *child1;
    /* 0x18 */ struct Node *child2;
} Node;

/* Clears flag 1 over the subtree. */
void func_0026CEC8(Node *arg0) {
    u16 temp_v1;
    Node *temp_a0;
    Node *temp_a0_2;
    Node *temp_a0_3;

    temp_v1 = arg0->flags;
    if (temp_v1 & 1) {
        temp_a0 = arg0->child0;
        arg0->flags = (u16) (temp_v1 & 0xFFFE);
        if (temp_a0 != NULL) {
            func_0026CEC8(temp_a0);
        }
        temp_a0_2 = arg0->child1;
        if (temp_a0_2 != NULL) {
            func_0026CEC8(temp_a0_2);
        }
        temp_a0_3 = arg0->child2;
        if (temp_a0_3 != NULL) {
            func_0026CEC8(temp_a0_3);
        }
    }
}
