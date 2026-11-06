#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

f32 func_00298470(f32);   /* sqrtf */
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

/* Sweep against a vertical cylinder of the given radius round centre
 * (optionally only between minY and maxY): 1 if it is hit before maxT and
 * Sweep_HitPlane accepts the hit. */
s32 Sweep_HitCylinder(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside) {
    CVector3 delta;
    CVector3 dir;
    CVector3 perp;
    CVector3 hit;
    CVector3 normal;
    f32 len;
    f32 invLen;
    f32 along;
    f32 disc;
    f32 t;

    if (sweep->lastKind == 2 && sweep->lastRadius == radius && sweep->lastContact.x == centre->x &&
        sweep->lastContact.y == centre->y && sweep->lastContact.z == centre->z) {
        return 0;
    }
    delta.x = centre->x - sweep->inst->m_vPos.x;
    delta.z = centre->z - sweep->inst->m_vPos.z;
    if (frontOnly && delta.x * sweep->delta.x + delta.z * sweep->delta.z < 0.0f) {
        return 0;
    }
    len = func_00298470(sweep->delta.x * sweep->delta.x + sweep->delta.z * sweep->delta.z);
    if (len == 0.0f) {
        return 0;
    }
    invLen = 1.0f / len;
    dir.x = sweep->delta.x * invLen;
    dir.z = sweep->delta.z * invLen;
    along = delta.x * dir.x + delta.z * dir.z;
    perp.x = delta.x - dir.x * along;
    perp.z = delta.z - dir.z * along;
    disc = radius * radius - (perp.x * perp.x + perp.z * perp.z);
    if (!(disc > 0.0f)) {
        return 0;
    }
    t = (along - func_00298470(disc)) * invLen;
    if (t < 0.0f && !fromInside) {
        return 0;
    }
    if (!(t < sweep->hit.fraction)) {
        return 0;
    }
    CVector3__Lerp(&hit, t, &sweep->inst->m_vPos, &sweep->end);
    if (checkHeight) {
        if (maxY < hit.y) {
            return 0;
        }
        if (hit.y < minY) {
            return 0;
        }
    }
    normal.x = hit.x - centre->x;
    normal.y = 0.0f;
    normal.z = hit.z - centre->z;
    if (Sweep_HitPlane(sweep, &normal, &hit, 0, arg7, arg8, 1) == 0) {
        return 0;
    }
    sweep->kind = 2;
    sweep->contact = *centre;
    sweep->radius = radius;
    return 1;
}
