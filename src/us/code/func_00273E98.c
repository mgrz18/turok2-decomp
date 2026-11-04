#include "common.h"

typedef struct { s32 x, y, z; } Vec3i;

/* Only what this writes. */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ Vec3i pos;
    /* 0x50 */ s32 state;
} Channel;

void func_00273E98(Channel *arg0, Vec3i *arg1, s32 arg2) {
    arg0->pos = *arg1;
    arg0->state = arg2;
}
