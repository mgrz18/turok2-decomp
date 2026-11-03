/* First VM module copy of func_00468264 (same source); the per-timer calls are inline helpers, which keeps GCC from strength-reducing &s->a[i]. */
#include "common.h"

/* The module's timer state, D_800F56B0. */
typedef struct Timer {
    /* 0x00 */ s32 value;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 reload;
    /* 0x0C */ s32 count;
    /* 0x10 */ s32 index;
    /* 0x14 */ void (*callback)(struct Timer *);
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Timer; /* size 0x20 */

typedef struct TimerState {
    /* 0x000 */ s32 active;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
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


/* One per player, 0x224 bytes apart. */
typedef struct Slot {
    /* 0x00 */ s32 a;
    /* 0x04 */ u8 pad4[0x12];
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 c;
    /* 0x18 */ u8 pad18[0x224 - 0x18];
} Slot;

extern TimerState D_800F56B0;
void func_0042F1C4(void);
extern Slot D_800F5F70[];
static __inline__ void fire(Timer *t) { t->callback(t); }
static __inline__ void tick(Timer *t) { if (--t->count <= 0) t->callback(t); }
void func_0042F344(void) {
    TimerState *s = &D_800F56B0;
    s32 i;

    if (s->unkC != 0) {
        for (i = 0; i < s->count; i++) {
            fire(&s->a[i]);
            fire(&s->b[i]);
            fire(&s->c[i]);
        }
    } else {
        for (i = 0; i < s->count; i++) {
            tick(&s->a[i]);
            tick(&s->b[i]);
            tick(&s->c[i]);
        }
    }
    for (i = 0; i < s->count; i++) {
        if (s->done != 0) {
            D_800F5F70[i].a = 0;
            D_800F5F70[i].b = 0;
            D_800F5F70[i].c = 0;
        } else {
            D_800F5F70[i].a = s->a[i].value;
            D_800F5F70[i].b = s->b[i].value;
            D_800F5F70[i].c = s->c[i].value;
        }
    }
    func_0042F1C4();
}
