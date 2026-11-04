#include "common.h"

typedef struct Node {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;               /* 0x8000: dirty */
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Node *child0;
    /* 0x14 */ struct Node *child1;
    /* 0x18 */ struct Node *child2;
} Node;

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ Node *node;
} Arg0;

s32 func_00400000(s32);
s32 func_00401CE8(s32);

void func_00401B70(Arg0 *arg0) {
    Node *temp_s0;
    Node *temp_a0;
    Node *temp_a0_2;
    Node *temp_a0_3;

    func_00400000((s32) arg0->node);
    temp_s0 = arg0->node;
    if (temp_s0 != 0) {
        temp_a0 = temp_s0->child0;
        temp_s0->flags = (u16) (temp_s0->flags & 0x7FFF);
        if ((temp_a0 != NULL) && (temp_a0->flags & 0x8000)) {
            func_00401CE8((s32) temp_a0);
        }
        temp_a0_2 = temp_s0->child1;
        if ((temp_a0_2 != NULL) && (temp_a0_2->flags & 0x8000)) {
            func_00401CE8((s32) temp_a0_2);
        }
        temp_a0_3 = temp_s0->child2;
        if ((temp_a0_3 != NULL) && (temp_a0_3->flags & 0x8000)) {
            func_00401CE8((s32) temp_a0_3);
        }
    }
}
