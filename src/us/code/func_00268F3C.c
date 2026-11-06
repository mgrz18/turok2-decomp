/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8024D018_3.CPP, func_8024D728_de, with Turok 2's layouts (Turok 2 reads the normal from the region). */
#include "common.h"
#include "structs/game_object.h"


extern f32 gQuaternionSin;

void CGameRegion__GetNormal(CVector3 *normal, void *region);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);     /* cross product */
void CVector3__Normalize(CVector3 *v);                         /* normalize */
f32 Math_AcosLookup(f32 x);                            /* acos */
f32 func_002982F0(f32 x);                            /* sin */
f32 func_002974C0(f32 x);                            /* cos */

/* Tilt of an actor toward the floor normal under it. */
CQuaternion GameObject_GetFloorTilt(CGameObjectInstance *obj) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuaternion q;
    f32 angle;

    CGameRegion__GetNormal(&normal, obj->header.m_pCurrentRegion);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    CVector3__Cross(&axis, &up, &normal);
    CVector3__Normalize(&axis);
    angle = Math_AcosLookup(up.x * normal.x + up.y * normal.y + up.z * normal.z) * 0.5f;
    gQuaternionSin = func_002982F0(angle);
    q.x = axis.x * gQuaternionSin;
    q.y = axis.y * gQuaternionSin;
    q.z = axis.z * gQuaternionSin;
    q.w = func_002974C0(angle);
    return q;
}
