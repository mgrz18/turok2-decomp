#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259B30.c, func_00259B30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
void CVector3__Subtract(CVector3 *out, void *src, sptr a);
void CMtxF__InverseRotateVector(void *src, CVector3 *a, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
f32 Math_AcosLookup(f32 x);
f32 func_002982F0(f32 x);
f32 __builtin_fabsf(f32 x);
s32 Sound_CalcPan(s32 a0, u8 *obj) {
    CVector3 r;
    CVector3 d;
    CVector3 w;
    f32 s;
    f32 zero;
    if (obj == 0) {
        return 0x40;
    }
    CVector3__Subtract(&d, obj + 0x114, a0);
    if (d.x * d.x + d.z * d.z < 1.0e2f) {
        return 0x40;
    }
    CMtxF__InverseRotateVector(obj + 0x14C, &d, &w);
    w.y = 0.0f;
    zero = w.y;
    r.y = zero;
    r.x = zero;
    r.z = 1.0f;
    CVector3__Normalize(&w);
    s = func_002982F0(Math_AcosLookup(w.x * r.x + w.y * r.y + w.z * r.z));
    if (w.x == zero) {
        return 0x40;
    }
    if (zero < w.x) {
        if (s < zero) {
            s = -s;
        }
    } else {
        s = -__builtin_fabsf(s);
    }
    return (s16)(s32)(s * 64.0f + 64.0f);
}
