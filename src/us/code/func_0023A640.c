#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

/* Where the segment from sweep->inst's position to sweep->end first enters
 * box, as a fraction of dir (slab test): 1 and *t if it does before maxT. */
s32 func_0023A640(Sweep *sweep, CBox *box, f32 *t) {
    CVector3 hit;
    CVector3 *start;
    CVector3 *end;
    f32 d;

    end = &sweep->end;
    start = &sweep->inst->m_vPos;
    if (box->m_vMax.x >= start->x && box->m_vMax.y >= start->y && box->m_vMax.z >= start->z &&
        box->m_vMin.x <= start->x && box->m_vMin.y <= start->y && box->m_vMin.z <= start->z) {
        *t = 0.0f;
        return 1;
    }
    if (start->x != end->x) {
        if (start->x < end->x) {
            d = (box->m_vMin.x - start->x) / (end->x - start->x);
        } else {
            d = (start->x - box->m_vMax.x) / (start->x - end->x);
        }
        if (d < sweep->hit.fraction) {
            hit.y = start->y + d * sweep->delta.y;
            hit.z = start->z + d * sweep->delta.z;
            if (box->m_vMin.y <= hit.y && hit.y <= box->m_vMax.y && box->m_vMin.z <= hit.z && hit.z <= box->m_vMax.z) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->y != end->y) {
        if (start->y < end->y) {
            d = (box->m_vMin.y - start->y) / (end->y - start->y);
        } else {
            d = (start->y - box->m_vMax.y) / (start->y - end->y);
        }
        if (d < sweep->hit.fraction) {
            hit.x = start->x + d * sweep->delta.x;
            hit.z = start->z + d * sweep->delta.z;
            if (box->m_vMin.x <= hit.x && hit.x <= box->m_vMax.x && box->m_vMin.z <= hit.z && hit.z <= box->m_vMax.z) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->z == end->z) {
        return 0;
    }
    if (start->z < end->z) {
        d = (box->m_vMin.z - start->z) / (end->z - start->z);
    } else {
        d = (start->z - box->m_vMax.z) / (start->z - end->z);
    }
    if (d < sweep->hit.fraction) {
        hit.x = start->x + d * sweep->delta.x;
        hit.y = start->y + d * sweep->delta.y;
        if (box->m_vMin.x <= hit.x && hit.x <= box->m_vMax.x && box->m_vMin.y <= hit.y && hit.y <= box->m_vMax.y) {
            *t = d;
            return 1;
        }
    }
    return 0;
}
