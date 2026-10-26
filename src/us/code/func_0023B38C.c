#include "common.h"
#include "structs/game_object.h"

/* A segment swept from an instance's position to end, and what it last hit. */
typedef struct Sweep {
    /* 0x00 */ CInstanceHeader *inst;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x40];
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 dir;
    /* 0x64 */ CVector3 missCentre;     /* last cylinder found not hit */
    /* 0x70 */ CVector3 hitCentre;      /* cylinder hit */
    /* 0x7C */ f32 missRadius;
    /* 0x80 */ f32 hitRadius;
    /* 0x84 */ s32 missKind;
    /* 0x88 */ s32 hitKind;
} Sweep;

f32 func_00298470(f32);   /* sqrtf */
void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

/* Sweep against a vertical cylinder of the given radius round centre
 * (optionally only between minY and maxY): 1 if it is hit before maxT and
 * func_0023AB18 accepts the hit. */
s32 func_0023B38C(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
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

    if (sweep->missKind == 2 && sweep->missRadius == radius && sweep->missCentre.x == centre->x &&
        sweep->missCentre.y == centre->y && sweep->missCentre.z == centre->z) {
        return 0;
    }
    delta.x = centre->x - sweep->inst->m_vPos.x;
    delta.z = centre->z - sweep->inst->m_vPos.z;
    if (frontOnly && delta.x * sweep->dir.x + delta.z * sweep->dir.z < 0.0f) {
        return 0;
    }
    len = func_00298470(sweep->dir.x * sweep->dir.x + sweep->dir.z * sweep->dir.z);
    if (len == 0.0f) {
        return 0;
    }
    invLen = 1.0f / len;
    dir.x = sweep->dir.x * invLen;
    dir.z = sweep->dir.z * invLen;
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
    if (!(t < sweep->maxT)) {
        return 0;
    }
    func_0020EF8C(&hit, t, &sweep->inst->m_vPos, &sweep->end);
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
    if (func_0023AB18(sweep, &normal, &hit, 0, arg7, arg8, 1) == 0) {
        return 0;
    }
    sweep->hitKind = 2;
    sweep->hitCentre = *centre;
    sweep->hitRadius = radius;
    return 1;
}
