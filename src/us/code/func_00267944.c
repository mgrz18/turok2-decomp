/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0024D860.c, func_8024D870_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))


extern f32 D_800DD78C;

s32 func_0026AFC0(void *region);
void func_0026BD20(CVector3 *normal, void *region);
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);     /* cross product */
void func_0020F040(CVector3 *v);                         /* normalize */
f32 func_0021153C(f32 x);                            /* acos */
f32 func_002982F0(f32 x);                            /* sin */
f32 func_002974C0(f32 x);                            /* cos */
f32 func_0026A268(void *region, f32 x, f32 z, f32 toX, f32 toZ);  /* ground slope */
void func_0020DCC8(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b);           /* slerp */

static __inline__ s32 isHeld(CGameObjectInstance *obj) {
    return (obj->header.m_Type != 1) ? 0 : (obj->m_dwFlags & 1);
}

/* Tilt of an actor toward the floor normal under it. */
static __inline__ CQuaternion floorTilt(CGameObjectInstance *obj) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuaternion q;
    f32 angle;

    func_0026BD20(&normal, obj->header.m_pCurrentRegion);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    func_0020EFDC(&axis, &up, &normal);
    func_0020F040(&axis);
    angle = func_0021153C(up.x * normal.x + up.y * normal.y + up.z * normal.z) * 0.5f;
    D_800DD78C = func_002982F0(angle);
    q.x = axis.x * D_800DD78C;
    q.y = axis.y * D_800DD78C;
    q.z = axis.z * D_800DD78C;
    q.w = func_002974C0(angle);
    return q;
}

/* An actor's body lean on the ground: players blend the floor tilt with a lean along their heading. */
CQuaternion func_00267944(CGameObjectInstance *obj) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuaternion tilt;
    CQuaternion lean;
    CQuaternion result;
    f32 slope;
    f32 sine;
    f32 cosine;
    f32 angle;
    f32 half;
    CGameObjectInstance *self;

    if (obj->header.m_pCurrentRegion != NULL && isHeld(obj) == 0) {
        if (obj->header.m_Type == 1) {
            self = obj;
            if (obj->m_AI.m_dwFlags & 0xC0000) {
                goto identity;
            }
            if (obj->m_dwFlags & 0x300000) {
                if (func_0026AFC0(obj->header.m_pCurrentRegion) != 0) {
                    goto identity;
                }
                func_0026BD20(&normal, obj->header.m_pCurrentRegion);
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;
                func_0020EFDC(&axis, &up, &normal);
                func_0020F040(&axis);
                slope = func_0021153C(up.x * normal.x + up.y * normal.y + up.z * normal.z);
                D_800DD78C = func_002982F0(MIN(slope, 0.43633235f) * 0.375f);
                tilt.x = axis.x * D_800DD78C;
                tilt.y = axis.y * D_800DD78C;
                tilt.z = axis.z * D_800DD78C;
                tilt.w = func_002974C0(MIN(slope, 0.43633235f) * 0.375f);
                sine = func_002982F0(self->m_RotY);
                cosine = func_002974C0(self->m_RotY);
                slope = func_0026A268(obj->header.m_pCurrentRegion, obj->header.m_vPos.x, obj->header.m_vPos.z, obj->header.m_vPos.x - sine, obj->header.m_vPos.z - cosine);
                slope = MIN(slope, 25.0f) * 0.75f;
                half = 0.5f;
                angle = slope * half;
                D_800DD78C = func_002982F0(angle);
                lean.y = 0.0f;
                lean.x = cosine * D_800DD78C;
                lean.z = -sine * D_800DD78C;
                lean.w = func_002974C0(angle);
                func_0020DCC8(&result, half, &lean, &tilt);
                return result;
            }
        }
        return floorTilt(obj);
    }
identity:
    tilt.x = tilt.y = tilt.z = 0.0f;
    tilt.w = 1.0f;
    return tilt;
}
