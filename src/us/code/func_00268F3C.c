/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8024D018_3.CPP, func_8024D728_de, with Turok 2's layouts (Turok 2 reads the normal from the region). */
#include "common.h"
#include "structs/game_object.h"


extern f32 D_800DD78C;

void func_0026BD20(CVector3 *normal, void *region);
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);     /* cross product */
void func_0020F040(CVector3 *v);                         /* normalize */
f32 func_0021153C(f32 x);                            /* acos */
f32 func_002982F0(f32 x);                            /* sin */
f32 func_002974C0(f32 x);                            /* cos */

/* Tilt of an actor toward the floor normal under it. */
CQuaternion func_00268F3C(CGameObjectInstance *obj) {
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
