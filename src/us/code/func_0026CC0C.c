#include "common.h"

/* A node with up to three links (0x10-0x18), as in func_0026C8B0.c. */
typedef struct Node {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;           /* 0x8000: marked */
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Node *link0;
    /* 0x14 */ struct Node *link1;
    /* 0x18 */ struct Node *link2;
} Node;

/* Clears the 0x8000 mark on arg0 and every marked node it links to. */
void func_0026CC0C(Node *arg0) {
    Node *temp_a0;
    Node *temp_a0_2;
    Node *temp_a0_3;

    temp_a0 = arg0->link0;
    arg0->flags = (u16) (arg0->flags & 0x7FFF);
    if ((temp_a0 != NULL) && (temp_a0->flags & 0x8000)) {
        func_0026CC0C(temp_a0);
    }
    temp_a0_2 = arg0->link1;
    if ((temp_a0_2 != NULL) && (temp_a0_2->flags & 0x8000)) {
        func_0026CC0C(temp_a0_2);
    }
    temp_a0_3 = arg0->link2;
    if ((temp_a0_3 != NULL) && (temp_a0_3->flags & 0x8000)) {
        func_0026CC0C(temp_a0_3);
    }
}
