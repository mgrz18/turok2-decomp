#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D268.c, func_0028D268.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, u8 *b);
void *func_002017D4();
s32 func_002261A8(CWorld *lv, s32 unused, CVector3 *pt) {
    CVector3 d;
    f32 bestIdx = -1.0f;
    f32 bestD = 0.0f;
    s32 i = 0;
    s32 *t;
    s32 n;
    u8 *p;
    t = func_002017D4(func_002017D4(lv->unk_68), 1);
    n = t[1];
    p = (u8 *)(t + 2);
    if (n > 0) {
        do {
            f32 dd;
            func_0020EF2C(&d, pt, p);
            dd = d.x * d.x + d.y * d.y + d.z * d.z;
            if (bestIdx < 0.0f || dd < bestD) {
                bestIdx = (f32)i;
                bestD = dd;
            }
            i++;
            p += 0x1C; /* 0x38 in Rage Wars */
        } while (i < n);
    }
    return (s32)bestIdx;
}
