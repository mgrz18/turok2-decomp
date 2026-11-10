/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284064.c, func_00284064.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/particle.h"
#ifdef TARGET_PORT
#include "structs/camera.h"
/* the camera's field of view by member */
typedef struct B19Q2 {
    union {
        struct { u8 _hp_f6C[__builtin_offsetof(CCamera, m_Fov)]; f32 f6C; }; /* N64 0x6C */
    };
} B19Q2;
#else
typedef struct B19Q2 {
    u8 pad0[0x6C];
    f32 f6C;
} B19Q2;
#endif
extern f32 D_800A6404;
extern f32 D_800A6408;
extern f32 D_800A640C;
extern f32 D_800A6410;
extern f32 D_800A6414;
extern f32 D_800A6418;
extern f32 D_800A641C;
extern f32 D_800A6420;
extern f32 D_800A6424;
/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline f32 Ramp2(CROMParticleEffect *par, f32 xin) {
    register f32 x asm("$f4") = xin;
    f32 d;
    f32 k;
    d = x - D_800A6404;
    k = !(d < 0.0f) ? (!(D_800A640C < d * D_800A6408) ? (!(d < 0.0f) ? d * D_800A6410 : 0.0f) : D_800A640C) : 0.0f;
    {
        register f32 fb asm("$f2") = par->m_pDistanceRange->m_FarScale;
        register f32 fa asm("$f3") = par->m_pDistanceRange->m_NearScale;
        x = x * (fa + (fb - fa) * k) * D_800A6414;
    }
    return x;
}
f32 CParticle__GetDistanceScale(CParticle *o, f32 px, B19Q2 *q) {
    CROMParticleEffect *par = o->m_pEffect;
    register f32 x asm("$f5") = px;
    f32 t;
    if (par->m_dwFlags & 4) {
        t = Ramp2(par, x);
    } else {
        t = D_800A6418;
    }
    {
        f32 m = q->f6C;
        return t * D_800A6424 * (m + (D_800A641C - m) * D_800A6420);
    }
}
