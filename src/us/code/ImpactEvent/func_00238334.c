#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/collision.h"
#include "globals/player.h"
#include "protos/impact_event.h"


f32 CVector3__Distance(CVector3 *a, CVector3 *b);
s32 CGameObjectInstance__GetNearestBox(CGameObjectInstance *a, CVector3 pos, HitFrame *out);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *a);

/* How much of a blast at `p` reaches `a`: its armour, then (with falloff) a linear drop over
 * the distance to the nearest point of its body, out to `reach` plus its own size. */
f32 Blast_GetReach(CGameObjectInstance *a, CGameObjectInstance *t, CVector3 p, f32 reach, s32 falloff, u32 flags, HitFrame **psrc) {
    CVector3 c;
    CCollisionResults *o;
    HitFrame *sv;
    s32 armored;
    s32 ok;
    f32 m;
    f32 lo;
    f32 hi;
    f32 lim;
    f32 dist;

    m = 1.0f;
    armored = 1;
    ok = 1;
    if (gEngineApp_m_Settings.multiplayer != 0 && (flags & 0x400000)) {
        armored = 0;
    }
    if (t->header.m_Type == 2 && (CGameObjectInstance *)(o = &gCollisionResults)->m_pInstance == a && o->m_nInstanceNode != -1) {
        sv = &o->m_Frame;
        falloff = 0;
    } else if (CGameObjectInstance__GetNearestBox(a, p, &sBlastShieldFrame) != 0) {
        sv = &sBlastShieldFrame;
    } else {
        sv = NULL;
    }
    *psrc = sv;
    if (armored && sv != NULL) {
        m *= sv->damageFactor;
    }
    if (!falloff) {
        return m;
    }
    c = a->header.m_vPos;
    lo = a->header.m_vPos.y + a->m_CollisionYOffset;
    hi = lo + GameObject_GetCollisionHeight(a);
    if (hi < p.y) {
        c.y = hi;
    } else if (p.y < lo) {
        c.y = lo;
    } else {
        c.y = p.y;
    }
    lim = reach + GameObject_GetCollisionRadius(a);
    dist = CVector3__Distance(&p, &c);
    if (lim < dist) {
        return 0.0f;
    }
    if (ok) {
        m *= 1.0f - dist / lim;
    }
    return m;
}
