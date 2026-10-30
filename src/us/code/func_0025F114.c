#include "common.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ void *def;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x36];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 pad37[0xC7 - 0x37];
    /* 0xC7 */ s8 unkC7;
} AI;

/* The point turned to, as three globals (D_800B06A0 is a Vec3f elsewhere). */
extern f32 D_800B06A0;
extern f32 D_800B06A4;
extern f32 D_800B06A8;

f32 func_00246310(Actor *self, f32 x, f32 y, f32 z); /* angle to a point (a Vec3f by value) */
void func_00245BAC(Actor *self, AI *ai, f32 rate, f32 angle);
void func_0025D474(Actor *self, AI *ai);

/* Like func_0025EB24: the turn rate, pi/3 in modes 1-2, else pi/12. */
static __inline__ f32 TurnRate(Actor *self, AI *ai) {
    f32 rate = 0.261799425f;

    if (ai->unk36 == 1 || ai->unk36 == 2) {
        rate = 1.0471977f;
    }
    return rate;
}

/* AI callback: turns toward the point D_800B06A0, then calls func_0025D474
 * once 0xC7 reaches 5. */
void func_0025F114(Actor *self, AI *ai) {
    func_00245BAC(self, ai, TurnRate(self, ai), func_00246310(self, D_800B06A0, D_800B06A4, D_800B06A8));
    if (ai->unkC7 >= 5) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->def != NULL) {
            func_0025D474(self, ai);
        } else {
            func_0025D474(self, ai);
        }
    }
}
