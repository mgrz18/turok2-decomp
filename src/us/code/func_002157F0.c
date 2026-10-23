/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_8.CPP, func_8024F5A0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x164];
    /* 0x164 */ f32 scale;
    /* 0x168 */ u8 pad168[4];
    /* 0x16C */ u16 flags;
    /* 0x16E */ u8 pad16E[2];
    /* 0x170 */ f32 scaleTime;
} StaticObject;

extern f32 D_800B6D28; /* frame delta */

/* Grow-in effect: ramp the scale up to 0.35 over 0.75 time units. */
void func_002157F0(StaticObject *obj) {
    f32 t;

    if (obj->flags & 0x10) {
        t = obj->scaleTime + D_800B6D28 * 0.06666667f;
        obj->scaleTime = t;
        if (t < 0.75f) {
            obj->scale = t * 0.46666667f;
            return;
        }
        obj->flags &= ~0x10;
        obj->scale = 0.35f;
    }
}
