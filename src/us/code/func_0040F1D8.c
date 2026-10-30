#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x144 - 0xD8];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 value;
} Actor;

/* Sums `value` over the group, and separately over the members still alive with flag 0x100. */
void func_0040F1D8(void *m, s32 count, Actor **group, s32 *total, s32 *active) {
    Actor *a;
    s32 i;

    for (i = 0; i < count; i++) {
        a = group[i];
        *total += a->value;
        if (a->health != 0 && (a->flags & 0x100)) {
            *active += a->value;
        }
    }
}
