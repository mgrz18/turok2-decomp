#include "common.h"
#include "structs/game_object.h"

/* A rectangle on the ground plane. */
typedef struct Rect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Rect;

/* A segment swept from an instance's position to end. */
typedef struct Sweep {
    /* 0x00 */ CInstanceHeader *inst;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x40];
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 dir;
} Sweep;

/* func_0023A640 on the ground plane: where the segment from sweep->inst's
 * position to sweep->end first enters rect (x and z only). */
s32 func_0023A968(Sweep *sweep, Rect *rect, f32 *t) {
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
        if (d < sweep->maxT) {
            hit.z = start->z + d * sweep->dir.z;
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
    if (d < sweep->maxT) {
        hit.x = start->x + d * sweep->dir.x;
        if (rect->minX <= hit.x && hit.x <= rect->maxX) {
            *t = d;
            return 1;
        }
    }
    return 0;
}
