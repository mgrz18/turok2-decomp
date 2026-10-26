#include "common.h"
#include "structs/game_object.h"

typedef struct Box {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

/* A segment swept from an instance's position to end. */
typedef struct Sweep {
    /* 0x00 */ CInstanceHeader *inst;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x40];
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 dir;
} Sweep;

/* Where the segment from sweep->inst's position to sweep->end first enters
 * box, as a fraction of dir (slab test): 1 and *t if it does before maxT. */
s32 func_0023A640(Sweep *sweep, Box *box, f32 *t) {
    CVector3 hit;
    CVector3 *start;
    CVector3 *end;
    f32 d;

    end = &sweep->end;
    start = &sweep->inst->m_vPos;
    if (box->max.x >= start->x && box->max.y >= start->y && box->max.z >= start->z &&
        box->min.x <= start->x && box->min.y <= start->y && box->min.z <= start->z) {
        *t = 0.0f;
        return 1;
    }
    if (start->x != end->x) {
        if (start->x < end->x) {
            d = (box->min.x - start->x) / (end->x - start->x);
        } else {
            d = (start->x - box->max.x) / (start->x - end->x);
        }
        if (d < sweep->maxT) {
            hit.y = start->y + d * sweep->dir.y;
            hit.z = start->z + d * sweep->dir.z;
            if (box->min.y <= hit.y && hit.y <= box->max.y && box->min.z <= hit.z && hit.z <= box->max.z) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->y != end->y) {
        if (start->y < end->y) {
            d = (box->min.y - start->y) / (end->y - start->y);
        } else {
            d = (start->y - box->max.y) / (start->y - end->y);
        }
        if (d < sweep->maxT) {
            hit.x = start->x + d * sweep->dir.x;
            hit.z = start->z + d * sweep->dir.z;
            if (box->min.x <= hit.x && hit.x <= box->max.x && box->min.z <= hit.z && hit.z <= box->max.z) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->z == end->z) {
        return 0;
    }
    if (start->z < end->z) {
        d = (box->min.z - start->z) / (end->z - start->z);
    } else {
        d = (start->z - box->max.z) / (start->z - end->z);
    }
    if (d < sweep->maxT) {
        hit.x = start->x + d * sweep->dir.x;
        hit.y = start->y + d * sweep->dir.y;
        if (box->min.x <= hit.x && hit.x <= box->max.x && box->min.y <= hit.y && hit.y <= box->max.y) {
            *t = d;
            return 1;
        }
    }
    return 0;
}
