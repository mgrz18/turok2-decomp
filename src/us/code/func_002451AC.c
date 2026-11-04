/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00215B90.c, func_00215B90, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


s32 func_0024E700(CPlayer *player, s32 weapon);   /* ammo for a weapon */
f32 func_002671B4(CGameObjectInstance *a);                     /* height */
f32 func_0021153C(f32 x);                        /* acos */
f32 func_002982F0(f32 x);                        /* sin */
f32 func_002974C0(f32 x);                        /* cos */
f32 func_00298470(f32 x);                        /* sqrt */
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
void func_0020F040(CVector3 *v);                          /* normalise */
void func_0021F00C(CVector3 *out, CGameObjectInstance *a, CVector3 v);    /* v in a's frame */

/* func_00246310: the signed angle from the actor's heading to a point. */
static __inline__ f32 AngleTo(CGameObjectInstance *actor, CVector3 point) {
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
    angle = func_0021153C(dot);
    if (!(dx * c + dz * s > 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* Whether `self` sees `target` (a player holding a ready weapon, or a
 * creature in an attack pose) aiming at it: 0 if not, else the side (-1/1). */
s32 func_002451AC(CGameObjectInstance *self, s32 unused, CGameObjectInstance *target, f32 cone) {
    CVector3 dir;
    CVector3 facing;
    CVector3 fwd;
    CVector3 from;
    CVector3 to;
    f32 lo;
    f32 hi;
    f32 y;
    f32 angle;
    CPlayer *player;

    if (target == NULL) {
        return 0;
    }
    if (target->m_dwFlags & 0x300000) {
        player = ((CPlayer *)target->m_AI.m_pParent);
        if (func_0024E700(player, player->m_Data.m_CurrentWeapon) <= 0) {
            return 0;
        }
        if (!(player->m_ContState.m_dwButtons & 0x2000)) {
            return 0;
        }
        goto body;
    }
    switch (*((s32 *)target->header.m_pIntelligence)) {
    case 1:
        if (target->m_AI.m_AnimType < 0x10CC) {
            return 0;
        }
        goto body;
    case 4:
        if (target->m_AI.m_Mode != 0x3F) {
            return 0;
        }
        goto body;
    default:
        return 0;
    }
body:
    lo = self->header.m_vPos.y + self->m_CollisionYOffset;
    hi = lo + func_002671B4(self);
    to.x = target->header.m_vPos.x;
    y = target->header.m_vPos.y + target->m_CollisionYOffset + func_002671B4(target) * 0.9f;
    to.y = y;
    to.z = target->header.m_vPos.z;
    from.x = self->header.m_vPos.x;
    from.z = self->header.m_vPos.z;
    if (hi < y) {
        from.y = hi;
    } else if (y < lo) {
        from.y = lo;
    } else {
        from.y = y;
    }
    angle = (AngleTo(self, to) < 0.0f) ? -AngleTo(self, to) : AngleTo(self, to);
    if (1.2217306f < angle && angle < 1.9198624f) {
        return 0;
    }
    func_0020EF2C(&dir, &from, &to);
    func_0020F040(&dir);
    fwd.x = 0;
    fwd.y = 0;
    fwd.z = 1.0f;
    func_0021F00C(&facing, target, fwd);
    if (func_0021153C(dir.x * facing.x + dir.y * facing.y + dir.z * facing.z) < cone * 0.5f) {
        if (dir.x * -facing.z + dir.z * facing.x < 0.0f) {
            return -1;
        }
        return 1;
    }
    return 0;
}
