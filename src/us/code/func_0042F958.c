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
extern s32 D_8011AD10;

void func_0042F958(void) {
    TimerState *s = &D_800F56B0;
    Timer *t = &s->d;

    if (s->fastForward != 0 || --t->count <= 0) {
        t->callback(t);
    }
    D_8011AD10 = s->d.value;
}
