#include "common.h"

/* A node of a graph flood-filled by group: the group at 0, flags at 2 (1: visited), up to three neighbours at 0x10. */
typedef struct GraphNode {
    /* 0x00 */ u16 group;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ struct GraphNode *neighbours[3];
} GraphNode;

void func_0026CE44(GraphNode *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    GraphNode *temp_a0;
    GraphNode *temp_a0_2;
    GraphNode *temp_a0_3;

    temp_s0 = arg0->group;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->flags;
        if (!(temp_v1 & 1)) {
            temp_a0 = arg0->neighbours[0];
            arg0->flags = temp_v1 | 1;
            if (temp_a0 != NULL) {
                func_0026CE44(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->neighbours[1];
            if (temp_a0_2 != NULL) {
                func_0026CE44(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->neighbours[2];
            if (temp_a0_3 != NULL) {
                func_0026CE44(temp_a0_3, temp_s0);
            }
        }
    }
}
