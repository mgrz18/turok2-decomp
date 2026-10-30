#include "common.h"

typedef struct Actor004035F8 {
    /* 0x000 */ u8 pad0[0x144];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad1[0x28C - 0x148];
    /* 0x28C */ struct Actor004035F8 *next;
} Actor004035F8;

typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 alive;
} Member004035F8;

typedef struct {
    /* 0x000 */ u8 pad0[0x224];
    /* 0x224 */ Member004035F8 *members[8];
    /* 0x244 */ s32 count;
} Group004035F8;

extern Actor004035F8 *D_8010FEB4;

s32 func_004035F8(Group004035F8 *group) {
    s32 i;
    s32 n = 0;
    Actor004035F8 *a;

    for (i = 0; i < group->count; i++) {
        if (group->members[i]->alive == 1) {
            n++;
        }
    }
    for (a = D_8010FEB4; a != NULL; a = a->next) {
        if (a->health > 0) {
            n++;
        }
    }
    return n;
}
