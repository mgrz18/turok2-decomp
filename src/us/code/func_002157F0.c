/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_8.CPP, func_8024F5A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/level_actor.h"

extern f32 D_800B6D28; /* frame delta */

/* Grow-in effect: ramp the scale up to 0.35 over 0.75 time units. */
void func_002157F0(CLevelActor *obj) {
    f32 t;

    if (obj->m_wFlags & 0x10) {
        t = obj->m_Timer + D_800B6D28 * 0.06666667f;
        obj->m_Timer = t;
        if (t < 0.75f) {
            obj->m_ModelScale = t * 0.46666667f;
            return;
        }
        obj->m_wFlags &= ~0x10;
        obj->m_ModelScale = 0.35f;
    }
}
