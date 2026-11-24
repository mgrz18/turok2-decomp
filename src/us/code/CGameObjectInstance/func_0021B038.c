#include "structs/vector.h"
#include "structs/game_object.h"
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
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
/* The rotation through an out pointer: the N64 definition's struct return (its hidden first
 * argument) under its own name, the port's shim (port/game/t2abi.c, ABI.md struct_scalar). */
#ifdef TARGET_PORT
void CGameObjectInstance__GetRotation_sret(B10Q4 *out, CGameObjectInstance *o);
#define CGameObjectInstance__GetRotation_o CGameObjectInstance__GetRotation_sret
#else
void CGameObjectInstance__GetRotation_o(B10Q4 *out, CGameObjectInstance *o) __asm__("CGameObjectInstance__GetRotation");
#endif
f32 GameObject_GetCollisionHeight(void *obj);
void CVector3__Subtract(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Normalize(CVector3 *v);
void CMtxF__TransformPoint(void *m, void *in, void *out);
void CQuaternion__Multiply(void *out, void *a, void *b);
void CQuaternion__ToMatrix(void *q, void *m);
f32 Math_AcosLookup(f32 x);
f32 func_00298470(f32 x); /* sqrtf */
B10Q4 *CGameObjectInstance__GetAutoAimRotation(B10Q4 *out, CGameObjectInstance *self, CVector3 dir, B10Q4 *outq, f32 angle) {
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
    CGameObjectInstance *best;
    CGameObjectInstance *obj;
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
    CGameObjectInstance__GetRotation_o(&q70, self);
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
        if (obj->m_dwFlags & 1) {
            continue;
        }
        if (obj->m_AI.m_Health == 0) {
            continue;
        }
        v60 = obj->header.m_vPos;
        v60.y = v60.y + obj->m_CollisionYOffset;
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
