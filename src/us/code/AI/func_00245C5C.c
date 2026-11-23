/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002168B8.c, func_002168B8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_FindClearHeading AI_FindClearHeading_i3
#endif
f32 AI_FindClearHeading(void *, s32, s32, s32, void *, f32);
void Actor_TurnBy(void *, f32);
typedef struct { u8 pad0[4]; s32 x, y, z; } B04Tg; /* Turok 2: pos at 0x4 */
void AI_TurnTowardTarget(void *o, s32 a1, B04Tg *t, f32 lim) {
    f32 r, v, a;
    if (t != ((void *)0)) {
        r = AI_FindClearHeading(o, t->x, t->y, t->z, t, 1.5f);
        v = r;
        if (lim < r) r = lim;
        else if (r < -lim) r = -lim;
        r = r * gFrameIncrement;
        a = r;
        if (a < 0.0f) a = -a;
        if (v < 0.0f) {
            if (-v < a) goto blk;
        } else if (v < a) {
blk:
            r = v;
        }
        Actor_TurnBy(o, r);
    }
}
