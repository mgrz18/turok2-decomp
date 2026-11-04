#include "common.h"

/* A node with up to three links (0x10-0x18), as in func_0026C8B0.c. */
typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;           /* 1: marked */
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ s32 link0;
    /* 0x14 */ s32 link1;
    /* 0x18 */ s32 link2;
} Node;

/* The region info func_0022425C returns (as in func_0023B664.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0x58 - 0x48];
    /* 0x58 */ s8 unk58;
    /* 0x59 */ u8 pad59[2];
    /* 0x5B */ u8 flags5B;
    /* 0x5C */ s8 unk5C;
} RegionInfo;

RegionInfo *func_0022425C(void *, void *);          /* extern */
void func_0026CEC8(s32);                            /* extern */
extern u8 D_800F7078[];

void func_0026C678(Node *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    u16 temp_v1;
    RegionInfo *temp_v0;

    if (arg0 != NULL) {
        temp_v1 = arg0->flags;
        if (temp_v1 & 1) {
            temp_a0 = arg0->link0;
            arg0->flags = (u16) (temp_v1 & 0xFFFE);
            if (temp_a0 != 0) {
                func_0026CEC8(temp_a0);
            }
            temp_a0_2 = arg0->link1;
            if (temp_a0_2 != 0) {
                func_0026CEC8(temp_a0_2);
            }
            temp_a0_3 = arg0->link2;
            if (temp_a0_3 != 0) {
                func_0026CEC8(temp_a0_3);
            }
        }
        temp_v0 = func_0022425C(D_800F7078, arg0);
        if (temp_v0 != NULL) {
            temp_v0->unk58 = 0;
            temp_v0->unk5C = 0;
            temp_v0->flags = (s32) (temp_v0->flags & ~1);
            temp_v0->flags5B = (u8) (temp_v0->flags5B & 0xFD);
        }
    }
}
