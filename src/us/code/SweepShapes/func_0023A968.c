#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "structs/rect.h"

/* A rectangle on the ground plane. */
/* Sweep_HitBox on the ground plane: where the segment from sweep->inst's
 * position to sweep->end first enters rect (x and z only). */
s32 Sweep_HitRect(Sweep *sweep, CRect *rect, f32 *t) {
    CVector3 hit;
    CVector3 *start;
    CVector3 *end;
    f32 d;

    end = &sweep->end;
    start = &sweep->inst->m_vPos;
    if (rect->maxX >= start->x && rect->maxZ >= start->z && rect->minX <= start->x && rect->minZ <= start->z) {
        *t = 0.0f;
        return 1;
    }
    if (start->x != end->x) {
        if (start->x < end->x) {
            d = (rect->minX - start->x) / (end->x - start->x);
        } else {
            d = (start->x - rect->maxX) / (start->x - end->x);
        }
        if (d < sweep->hit.fraction) {
            hit.z = start->z + d * sweep->delta.z;
            if (rect->minZ <= hit.z && hit.z <= rect->maxZ) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->z == end->z) {
        return 0;
    }
    if (start->z < end->z) {
        d = (rect->minZ - start->z) / (end->z - start->z);
    } else {
        d = (start->z - rect->maxZ) / (start->z - end->z);
    }
    if (d < sweep->hit.fraction) {
        hit.x = start->x + d * sweep->delta.x;
        if (rect->minX <= hit.x && hit.x <= rect->maxX) {
            *t = d;
            return 1;
        }
    }
    return 0;
}
