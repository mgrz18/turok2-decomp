#include "common.h"

/* A node with up to three links (0x10-0x18) to nodes of the same id. */
typedef struct Node {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;           /* 4: visited */
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Node *link0;
    /* 0x14 */ struct Node *link1;
    /* 0x18 */ struct Node *link2;
} Node;

/* Marks arg0 and, recursively, every node linked to it with id arg1. */
void func_0026C8B0(Node *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    Node *temp_a0;
    Node *temp_a0_2;
    Node *temp_a0_3;

    temp_s0 = arg0->id;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->flags;
        if (!(temp_v1 & 4)) {
            temp_a0 = arg0->link0;
            arg0->flags = (u16) (temp_v1 | 4);
            if (temp_a0 != NULL) {
                func_0026C8B0(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->link1;
            if (temp_a0_2 != NULL) {
                func_0026C8B0(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->link2;
            if (temp_a0_3 != NULL) {
                func_0026C8B0(temp_a0_3, temp_s0);
            }
        }
    }
}
