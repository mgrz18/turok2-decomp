#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct BlastActor {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1[0x3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x54 - 0x10];
    /* 0x054 */ f32 footY;
} BlastActor;

extern CCollisionResults D_800F1CE0;  /* the last collision test: the node it hit (m_Frame) is the armour to use */
extern HitFrame D_800F1970;
extern u8 D_8011AAF1;

f32 func_0020F6BC(CVector3 *a, CVector3 *b);
s32 func_0021E374(BlastActor *a, CVector3 pos, HitFrame *out);
f32 func_002671B4(BlastActor *a);
f32 func_002672C8(BlastActor *a);

/* How much of a blast at `p` reaches `a`: its armour, then (with falloff) a linear drop over
 * the distance to the nearest point of its body, out to `reach` plus its own size. */
f32 func_00238334(BlastActor *a, BlastActor *t, CVector3 p, f32 reach, s32 falloff, u32 flags, HitFrame **psrc) {
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
    if (D_8011AAF1 != 0 && (flags & 0x400000)) {
        armored = 0;
    }
    if (t->kind == 2 && (BlastActor *)(o = &D_800F1CE0)->m_pInstance == a && o->m_nInstanceNode != -1) {
        sv = &o->m_Frame;
        falloff = 0;
    } else if (func_0021E374(a, p, &D_800F1970) != 0) {
        sv = &D_800F1970;
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
    hi = lo + func_002671B4(a);
    if (hi < p.y) {
        c.y = hi;
    } else if (p.y < lo) {
        c.y = lo;
    } else {
        c.y = p.y;
    }
    lim = reach + func_002672C8(a);
    dist = func_0020F6BC(&p, &c);
    if (lim < dist) {
        return 0.0f;
    }
    if (ok) {
        m *= 1.0f - dist / lim;
    }
    return m;
}
