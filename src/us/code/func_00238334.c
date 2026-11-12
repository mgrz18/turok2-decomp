#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/collision.h"
#include "globals/player.h"

#ifdef TARGET_PORT
typedef struct BlastActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_BlastActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_BlastActor(header.m_Type)]; u8 kind; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_BlastActor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_footY[T2_HOFF_BlastActor(m_CollisionYOffset)]; f32 footY; }; /* N64 0x54 */
    };
} BlastActor;
#else
typedef struct BlastActor {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1[0x3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x54 - 0x10];
    /* 0x054 */ f32 footY;
} BlastActor;
#endif

f32 CVector3__Distance(CVector3 *a, CVector3 *b);
s32 CGameObjectInstance__GetNearestBox(BlastActor *a, CVector3 pos, HitFrame *out);
f32 GameObject_GetCollisionHeight(BlastActor *a);
f32 GameObject_GetCollisionRadius(BlastActor *a);

/* How much of a blast at `p` reaches `a`: its armour, then (with falloff) a linear drop over
 * the distance to the nearest point of its body, out to `reach` plus its own size. */
f32 Blast_GetReach(BlastActor *a, BlastActor *t, CVector3 p, f32 reach, s32 falloff, u32 flags, HitFrame **psrc) {
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
    if (t->kind == 2 && (BlastActor *)(o = &gCollisionResults)->m_pInstance == a && o->m_nInstanceNode != -1) {
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
    c = a->pos;
    lo = a->pos.y + a->footY;
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
