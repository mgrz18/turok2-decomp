#include "common.h"

/* Resource entry: a key, its flags and three child entries. */
typedef struct Entry0026C5C4 {
    /* 0x00 */ u16 key;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Entry0026C5C4 *child0;
    /* 0x14 */ struct Entry0026C5C4 *child1;
    /* 0x18 */ struct Entry0026C5C4 *child2;
} Entry0026C5C4;

void CGameRegion__FloodClearFlag4Same(Entry0026C5C4 *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    Entry0026C5C4 *temp_a0;
    Entry0026C5C4 *temp_a0_2;
    Entry0026C5C4 *temp_a0_3;

    temp_s0 = arg0->key;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->flags;
        if (temp_v1 & 4) {
            temp_a0 = arg0->child0;
            arg0->flags = (u16) (temp_v1 & 0xFFFB);
            if (temp_a0 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->child1;
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->child2;
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0_3, temp_s0);
            }
        }
    }
}
