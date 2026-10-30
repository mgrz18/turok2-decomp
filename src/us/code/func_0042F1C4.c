#include "common.h"

/* A timer whose durations are read from a bit stream (see func_0042EC30). */
typedef struct Timer {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 *data;
    /* 0x08 */ u32 bitPos;
    /* 0x0C */ s32 count;
    /* 0x10 */ s32 index;
    /* 0x14 */ void (*callback)(struct Timer *);
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Timer; /* size 0x20 */

/* The module's state, D_800F56B0. */
typedef struct TimerState {
    /* 0x000 */ s32 active;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 fastForward;
    /* 0x010 */ s32 done;
    /* 0x014 */ Timer a[4];
    /* 0x094 */ Timer b[4];
    /* 0x114 */ Timer c[4];
    /* 0x194 */ Timer d;
    /* 0x1B4 */ s32 count;
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ s32 countdown;
    /* 0x1C0 */ f32 time;
    /* 0x1C4 */ s32 checksum;
} TimerState;

extern TimerState D_800F56B0;
extern s32 D_800B6D20;
extern f32 D_800B6D28;
extern s32 D_800F5D2C;
extern char D_800F6CB0[];

void func_00430330(void *);
void func_00288C5C(void *, s32);

/* func_0042F868 */
static __inline__ void updateChecksum(void) {
    TimerState *s = &D_800F56B0;
    s32 i;

    for (i = 0; i < s->count; i++) {
        s->checksum += D_800B6D20 * s->a[i].value * 0x12345678;
        s->checksum += D_800B6D20 * s->b[i].value * 0x87654321;
        s->checksum += D_800B6D20 * s->c[i].value * 0x12348765;
    }
    s->checksum += D_800B6D20 * s->d.value * 0x87651234;
}

void func_0042F1C4(void) {
    TimerState *s = &D_800F56B0;

    s->fastForward = 0;
    s->time += D_800B6D28;
    if (s->done == 0) {
        if (--s->countdown <= 0 || D_800F5D2C != 0) {
            s->done = 1;
            func_00430330(D_800F6CB0);
            if (D_800F5D2C != 0) {
                func_00288C5C(D_800F6CB0, 8);
            }
        }
    }
    updateChecksum();
}
