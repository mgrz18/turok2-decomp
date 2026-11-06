#include "common.h"

typedef struct {
    /* 0x00 */ char pad[0x64];
    /* 0x64 */ f32 v;
} Obj64;

extern f32 D_800A7948;
extern f32 D_800B6D28;
extern void CAI__SetMode(void *, void *, s32);

/* Has the shape of an AI step function, stepFunc(pThis, pAI) in LibTEngine:
 * clamp a timer at 0x64, count it down, and change mode when it runs out. */
void EnemyMode_ReactionBlinded_Update(void *self, Obj64 *o) {
    if (o->v > D_800A7948) {
        o->v = D_800A7948;
    }
    o->v -= D_800B6D28;
    if (o->v <= 0.0f) {
        CAI__SetMode(self, o, 10);
    }
}
