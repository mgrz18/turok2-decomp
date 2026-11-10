#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/math.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00247BA4.c, func_00247BA4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B10Q4 {
    f32 x, y, z, w;
} B10Q4;
typedef struct B10Mt {
    f32 m[16];
} B10Mt;
/* Turok 2 layout (Rage Wars: pos 0x8, f70 0x70, flags 0x100, f174 0x174). */
typedef struct B10Obj {
    u8 pad0[0x4];
    CVector3 pos;              /* 0x004 */
    u8 pad10[0x54 - 0x10];
    f32 f54;                /* 0x054 */
    u8 pad58[0xD4 - 0x58];
    u32 flags;              /* 0x0D4 */
    u8 padD8[0x144 - 0xD8];
    s32 f144;               /* 0x144 */
} B10Obj;
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar) */
#define CGameObjectInstance__GetRotation CGameObjectInstance__GetRotation_sret
#endif
void CGameObjectInstance__GetRotation(B10Q4 *out, B10Obj *o);
f32 GameObject_GetCollisionHeight(B10Obj *o);
void CVector3__Subtract(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Normalize(CVector3 *v);
void CMtxF__TransformPoint(B10Mt *m, CVector3 *a, CVector3 *b);
void CQuaternion__Multiply(B10Q4 *o, B10Q4 *a, B10Q4 *b);
void CQuaternion__ToMatrix(B10Q4 *q, B10Mt *m);
f32 Math_AcosLookup(f32 x);
f32 func_00298470(f32 x); /* sqrtf */
B10Q4 *func_0021B038(B10Q4 *out, B10Obj *self, CVector3 dir, B10Q4 *outq, f32 angle) {
    CVector3 v10;
    CVector3 v20;
    CVector3 v30;
    CVector3 v40;
    CVector3 v50;
    CVector3 v60;
    B10Q4 q70;
    B10Q4 q80;
    B10Q4 q90;
    B10Mt mA0;
    B10Obj *best;
    B10Obj *obj;
    s32 i;
    s32 n;
    f32 thr;
    f32 d;
    f32 t;
    f32 s;
    f32 new_var;
    f32 new_var2;
    new_var2 = angle;
    v20.x = 0;
    v20.y = 0;
    v20.z = 1.0f;
    CGameObjectInstance__GetRotation(&q70, self);
    if (new_var2 == 0.0f) {
        if (outq) {
            outq->x = outq->y = outq->z = 0;
            outq->w = 1.0f;
        }
        *out = q70;
        return out;
    }
    CQuaternion__ToMatrix(&q70, &mA0);
    CMtxF__TransformPoint(&mA0, &v20, &v30);
    best = 0;
    thr = func_002974C0(new_var2 * 0.008726647f);
    n = gScene.m_nRegistered;
    for (i = 0; i < n; i++) {
        obj = gScene.m_pRegistered[i];
        if (obj == self) {
            continue;
        }
        if (obj->flags & 1) {
            continue;
        }
        if (obj->f144 == 0) {
            continue;
        }
        v60 = obj->pos;
        v60.y = v60.y + obj->f54;
        v60.y = v60.y + GameObject_GetCollisionHeight(obj) * 0.5f;
        CVector3__Subtract(&v10, &v60, &dir);
        CVector3__Normalize(&v10);
        d = v30.x * v10.x + v30.y * v10.y + v30.z * v10.z;
        if (thr < d) {
            thr = d;
            v50 = v10;
            best = obj;
        }
    }
    if (best) {
        CVector3__Cross(&v40, &v30, &v50);
        CVector3__Normalize(&v40);
        t = Math_AcosLookup(thr);
        t *= func_00298470(func_00298470(1.0f - t / (new_var2 * 0.008726647f)));
        t *= 0.5f;
        s = func_002982F0(t);
        gQuaternionSin = s;
        q80.x = v40.x * s;
        q80.y = v40.y * s;
        q80.z = v40.z * s;
        new_var = t;
        q80.w = func_002974C0(new_var);
        CQuaternion__Multiply(&q90, &q70, &q80);
        if (outq) {
            *outq = q80;
        }
        *out = q90;
        return out;
    }
    if (outq) {
        outq->x = outq->y = outq->z = 0.0f;
        outq->w = 1.0f;
    }
    *out = q70;
    return out;
}
