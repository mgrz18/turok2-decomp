#include "common.h"

typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
} Vertex;

/* A node with three vertices and up to three links (0x10-0x18), as in
 * func_0026C8B0.c. */
typedef struct Node {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;
    /* 0x04 */ Vertex *verts[3];
    /* 0x10 */ struct Node *link0;
    /* 0x14 */ struct Node *link1;
    /* 0x18 */ struct Node *link2;
} Node;

/* Sets the height of arg0's vertices to arg1 and spreads it to every node
 * with id arg2 linked to it. */
void func_0026CA3C(Node *arg0, f32 arg1, s32 arg2) {
    Node *temp_a0;
    Node *temp_a0_2;
    Node *temp_a0_3;

    if (arg0->id == (arg2 & 0xFFFF)) {
        if (arg0->verts[0]->y == arg1) {
            if ((arg0->verts[1]->y != arg1) || (arg0->verts[2]->y != arg1)) {
                goto block_5;
            }
        } else {
block_5:
            arg0->verts[0]->y = arg1;
            arg0->verts[1]->y = arg1;
            arg0->verts[2]->y = arg1;
            temp_a0 = arg0->link0;
            if (temp_a0 != NULL) {
                func_0026CA3C(temp_a0, arg1, arg2 & 0xFFFF);
            }
            temp_a0_2 = arg0->link1;
            if (temp_a0_2 != NULL) {
                func_0026CA3C(temp_a0_2, arg1, arg2 & 0xFFFF);
            }
            temp_a0_3 = arg0->link2;
            if (temp_a0_3 != NULL) {
                func_0026CA3C(temp_a0_3, arg1, arg2 & 0xFFFF);
            }
        }
    }
}
