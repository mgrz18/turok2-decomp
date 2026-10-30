/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in func_0040C4F4); the plain
 * loop shifts its registers by one. */
#include "common.h"

/* Picks the next attack anim from a weighted table: close to the target (or while anim 0x17 runs its first
 * 100 steps) 0x1A/0x1B; otherwise 0x18 after a long 0x17, and from afar 0x19 (when fewer than 2 actors are
 * in the D_8010FEB4 list), 0x1D and 0x17 weighted by the module's timer ratio. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *callback234;
    /* 0x238 */ u8 pad238[0x248 - 0x238];
    /* 0x248 */ void *callback248;
    /* 0x24C */ u8 pad24C[0x250 - 0x24C];
    /* 0x250 */ s32 unk250;
    /* 0x254 */ s32 unk254;
} LevelActor;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ LevelActor *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 unk664;
    /* 0x668 */ u8 pad668[0x66C - 0x668];
    /* 0x66C */ s32 lastAnim;
    /* 0x670 */ u8 pad670[0x674 - 0x670];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x28C - 0xD8];
    /* 0x28C */ struct Actor *next;
} Actor;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Actor *actor;
    /* 0x08 */ u8 pad8[0x40 - 0x8];
    /* 0x40 */ f32 dist;
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u32 flags;
} CAI;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ CAI ai;
} Obj;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

extern State D_8012FB70;
extern Actor *D_8010FEB4;
extern void func_00246108(Obj *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern s16 func_0026D518(Pick *table);
extern void func_00243414(Obj *obj, CAI *ai, s32 state);

/* func_0040BD60 */
static __inline__ f32 getTargetDist(Obj *obj, Actor **found) {
    Targets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, &obj->ai, &t, 1, 0);
    if (t.b.actor != NULL && (t.b.actor->flags & 0x300000)) {
        dist = t.b.dist;
        *found = t.b.actor;
    }
    return dist;
}

/* func_0040C4F4 */
static __inline__ s32 fewerActorsThan(s32 max) {
    Actor *actor;
    s32 count = 0;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->next != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}

void func_0040A8FC(Obj *obj, CAI *ai) {
    Pick picks[8];
    Actor *found;
    State *state = &D_8012FB70;
    s32 n;
    s32 close;
    s32 far;
    s32 mid;
    s32 low;
    s32 veryLow;
    f32 dist;
    f32 ratio;

    n = veryLow = low = mid = far = close = 0;
    dist = getTargetDist(obj, &found);
    if (state->unk674 != 0) {
        ratio = (f32)state->unk678 / (f32)state->unk674;
    } else {
        ratio = 1.0f;
    }
    if (ratio < 0.4f) {
        low = 1;
    }
    if (ratio < 0.6f) {
        veryLow = 1;
    }
    if (found != NULL) {
        if (state->lastAnim == 0x17) {
            if (state->counter < 101) {
                close = 1;
            } else {
                picks[n].id = 0x18;
                picks[n++].weight = 5;
            }
        } else if (dist < 460.8f) {
            close = 1;
        } else {
            far = mid = 1;
        }
        if (close) {
            picks[n].id = 0x1A;
            picks[n++].weight = 5;
            picks[n].id = 0x1B;
            picks[n++].weight = 5;
        } else {
            if (far) {
                if (fewerActorsThan(2)) {
                    picks[n].id = 0x19;
                    picks[n++].weight = 4;
                }
                picks[n].id = 0x1D;
                picks[n++].weight = 6;
            }
            if (mid) {
                picks[n].id = 0x17;
                if (low) {
                    picks[n].weight = 6;
                } else if (veryLow) {
                    picks[n].weight = 9;
                } else {
                    picks[n].weight = 12;
                }
                n++;
            }
        }
        picks[n].id = -1;
        state->lastAnim = func_0026D518(picks);
        state->counter = 0;
        func_00243414(obj, ai, state->lastAnim);
    }
}
