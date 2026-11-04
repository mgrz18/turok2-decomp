#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028CE70.c, func_0028CE70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/world.h"
s32 func_00266D8C(s32 *ents, s32 n, s32 id, s32 *lo, s32 *hi);
void func_0026EBDC(s32 a, u8 *p, CVector3 v, f32 f, f32 g, s32 h);
void func_00225DB0(CWorld *lv, s32 a1, s32 id, CVector3 v, f32 f6, f32 f7) {
    s32 lo;
    s32 hi;
    s32 i;
    s32 *t = lv->m_pRumbleTypes;
    s32 n = t[1];
    if (func_00266D8C(t + 2, n, id, &lo, &hi) != 0) {
        i = lo;
        if (hi >= i) {
            do {
                s32 *tbl = lv->m_pRumbles;
                func_0026EBDC(a1, (u8 *)tbl + (i * tbl[0] + 8), v, f6, f7, 0);
                i++;
            } while (hi >= i);
        }
    }
}
