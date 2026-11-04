#include "common.h"

/* A node of a graph flood-filled by group: the group at 0, flags at 2 (1: visited, 0x8000: marked), up to three neighbours at 0x10. */
typedef struct GraphNode {
    /* 0x00 */ u16 group;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ struct GraphNode *neighbours[3];
} GraphNode;

void func_00401CE8(GraphNode *arg0) {
    GraphNode *temp_a0;
    GraphNode *temp_a0_2;
    GraphNode *temp_a0_3;

    if (arg0 != NULL) {
        temp_a0 = arg0->neighbours[0];
        arg0->flags &= 0x7FFF;
        if ((temp_a0 != NULL) && (temp_a0->flags & 0x8000)) {
            func_00401CE8(temp_a0);
        }
        temp_a0_2 = arg0->neighbours[1];
        if ((temp_a0_2 != NULL) && (temp_a0_2->flags & 0x8000)) {
            func_00401CE8(temp_a0_2);
        }
        temp_a0_3 = arg0->neighbours[2];
        if ((temp_a0_3 != NULL) && (temp_a0_3->flags & 0x8000)) {
            func_00401CE8(temp_a0_3);
        }
    }
}
