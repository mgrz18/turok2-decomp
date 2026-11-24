#include "structs/vector.h"
#include "structs/camera.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259B30.c, func_00259B30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CMtxF__InverseRotateVector(CMtxF *m, CVector3 *a, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
f32 Math_AcosLookup(f32 x);
f32 func_002982F0(f32 x);
f32 __builtin_fabsf(f32 x);
/* The pan (0..0x80) of a sound at pos heard from the camera (round 29, part r2: typed as the
 * port's copy in port/audio/t2audio_world.c). */
s32 Sound_CalcPan(CVector3 *pos, CCamera *cam) {
    CVector3 r;
    CVector3 d;
    CVector3 w;
    f32 s;
    f32 zero;
    if (cam == NULL) {
        return 0x40;
    }
    CVector3__Subtract(&d, &cam->m_vPos, pos);
    if (d.x * d.x + d.z * d.z < 1.0e2f) {
        return 0x40;
    }
    CMtxF__InverseRotateVector(&cam->m_mfViewOrient, &d, &w);
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
