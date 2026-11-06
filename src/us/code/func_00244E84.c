#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/collision.h"



f32 func_00298470(f32);         /* sqrtf */
f32 func_002982F0(f32);         /* sinf */
f32 func_002974C0(f32);         /* cosf */
f32 Math_AcosLookup(f32);         /* acosf */
f32 GameObject_GetCollisionRadius(CGameObjectInstance *a);    /* radius */
s32 GameObject_TestMove(CGameObjectInstance *self, CVector3 pos, s32 *out); /* blocked at pos */
void Angle_Wrap(f32 *angle); /* wrap */

/* AI_AngleToPoint: the angle from the actor's heading to a point. */
static __inline__ f32 angleTo(CGameObjectInstance *actor, CVector3 point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;

    dx = point.x - actor->header.m_vPos.x;
    dz = point.z - actor->header.m_vPos.z;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(actor->m_RotY);
    } while (0);
    c = -func_002974C0(actor->m_RotY);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = Math_AcosLookup(dot);
    if (!(dx * c + dz * s > 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* The heading for self toward pos: straight at it, or when pos is blocked,
 * the first free direction 30, 60, ... 150 degrees to either side (the side
 * tried first alternating between actors), turned 15 degrees further. */
f32 AI_FindClearHeading(CGameObjectInstance *self, CVector3 pos, CGameObjectInstance *target, f32 scale) {
    CVector3 probe;
    s32 out;
    f32 heading;
    f32 angle;
    f32 step;
    f32 extra;
    s32 i;
    s32 side;
    s32 first;

    angle = angleTo(self, pos);
    if ((self->m_dwFlags & 0x200) && GameObject_TestMove(self, pos, &out)) {
        if (target != NULL && gCollisionResults.m_pInstance == target) {
            return angle;
        }
        probe.y = self->header.m_vPos.y;
        first = ((u32) self % 0x288) & 1;
        for (i = 1; i < 6; i++) {
            step = i * 0.52359885f;
            for (side = 0; side < 2; side++) {
                heading = angle;
                if (side != first) {
                    heading = angle + step;
                    extra = 0.26179942f;
                } else {
                    heading = angle - step;
                    extra = -0.26179942f;
                }
                probe.x = self->header.m_vPos.x + func_002982F0(self->m_RotY + 3.1415927f + heading) * (scale * GameObject_GetCollisionRadius(self));
                probe.z = self->header.m_vPos.z + func_002974C0(self->m_RotY + 3.1415927f + heading) * (scale * GameObject_GetCollisionRadius(self));
                if (!GameObject_TestMove(self, probe, NULL)) {
                    heading += extra;
                    Angle_Wrap(&heading);
                    return heading;
                }
            }
        }
    }
    return angle;
}
