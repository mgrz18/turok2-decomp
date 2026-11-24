#include "common.h"
/* FAKEMATCH (round 21 audit): 2 register pins (`register ... asm("$n")`) and 1 empty `__asm__` barrier;
 * without them: func_00253934 14 of 22 words (size 0x54 vs 0x58). */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022AA34.c, func_0022AA34.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Actor Actor;
struct Actor { /* Turok 2 layout */
    u8 pad0[0xBE4];
    f32 unkBE4;         /* Rage Wars: 0x840 */
    u8 padBE8[4];
    s32 unkBEC;         /* Rage Wars: 0x848 */
};
f32 func_00253934(Actor *a) {
    f32 t = a->unkBE4 - 585.0f;
    register f32 tmp asm("$f0");
    register f32 v asm("$f1");
    if (a->unkBEC != 0) goto c0;
    tmp = t * 0.06666667f;
    __asm__("" : "+f"(tmp));
    v = tmp;
    if (1.0f < v) {
c0:
        v = 1.0f;
    }
    return 1.0f - v;
}
