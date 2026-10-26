#include "common.h"
#include "structs/game_object.h"

typedef struct Sphere {
    /* 0x00 */ CVector3 center;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 radiusSq;
} Sphere;

f32 func_00298470(f32);   /* sqrtf */

/* Whether the segment from a to b crosses the circle of sphere on the ground
 * plane (x and z only). */
s32 func_0023CF04(Sphere *sphere, CVector3 *a, CVector3 *b) {
    CVector3 toCenter;
    CVector3 dir;
    CVector3 perp;
    CVector3 seg;
    f32 len;
    f32 invLen;
    f32 along;
    f32 disc;
    f32 t;

    seg.x = b->x - a->x;
    seg.z = b->z - a->z;
    toCenter.x = sphere->center.x - a->x;
    toCenter.z = sphere->center.z - a->z;
    len = func_00298470(seg.x * seg.x + seg.z * seg.z);
    if (len == 0.0f) {
        return 0;
    }
    invLen = 1.0f / len;
    dir.x = seg.x * invLen;
    dir.z = seg.z * invLen;
    along = toCenter.x * dir.x + toCenter.z * dir.z;
    perp.x = toCenter.x - dir.x * along;
    perp.z = toCenter.z - dir.z * along;
    disc = sphere->radiusSq - (perp.x * perp.x + perp.z * perp.z);
    if (!(disc > 0.0f)) {
        return 0;
    }
    t = (along - func_00298470(disc)) * invLen;
    return t >= 0.0f && t <= 1.0f;
}
