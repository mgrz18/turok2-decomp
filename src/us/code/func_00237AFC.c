/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM_2/802661FC_2.CPP, func_802676EC_de (inlined here), with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

/* A blast: radius, damage, then the push (strength, x, y, z). */
typedef struct Blast {
    u16 radius;
    u16 damage;
    s8 strength;
    s8 x;
    s8 y;
    s8 z;
} __attribute__((aligned(4))) Blast;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x996 - 0x10];
    /* 0x996 */ u16 weapon;
} BlastOwner;

typedef struct {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x54 - 0x10];
    /* 0x054 */ f32 height;
    /* 0x058 */ u8 pad58[0xD4 - 0x58];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x1A8 - 0x140];
    /* 0x1A8 */ BlastOwner *owner;
} BlastActor;

extern u8 D_8011AAF1;           /* multiplayer */
extern CCollisionResults gCollisionResults;  /* the last collision test (m_pInstance: what it hit) */
extern HitFrame D_800F1970;       /* the shield CGameObjectInstance__GetNearestBox finds */

s32 Blast_CanHit(BlastActor *victim, BlastActor *source);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
f32 GameObject_GetCollisionHeight(void *actor);
f32 CGameObjectInstance__GetAverageScale(BlastActor *actor);
s32 CGameObjectInstance__GetNearestBox(BlastActor *actor, CVector3 pos, HitFrame *out);
f32 GameObject_GetCollisionRadius(BlastActor *actor);
f32 CVector3__Distance(CVector3 *a, CVector3 *b);
void Blast_GetPush(BlastActor *victim, BlastActor *source, Blast blast, CVector3 *out, u32 flags);
void DamageEvent_Set(DamageEvent *out, void *source, s32 damage, f32 radius, u32 flags, HitFrame *shield);
void CAI__CallHurt(BlastActor *victim, void *ai, DamageEvent *hit);
void Blast_ApplyHit(BlastActor *victim, BlastActor *source, s32 hit, CVector3 pos, CVector3 dir, s32 push, HitFrame *shield, u32 flags);

/* How strongly the blast reaches the victim: through its shield, and with
 * falloff over the radius plus its width from the nearest point of its body. */
static __inline__ f32 blastScale(BlastActor *victim, BlastActor *source, CVector3 position, f32 radius, s32 falloff, u32 flags, HitFrame **found) {
    s32 shielded;
    s32 scaled;
    HitFrame *shield;
    f32 scale;
    f32 bottom;
    f32 top;
    f32 reach;
    f32 distance;
    CVector3 body;

    shielded = 1;
    scale = 1.0f;
    scaled = 1;
    if (D_8011AAF1 != 0 && (flags & 0x400000)) {
        shielded = 0;
    }
    if (source->kind == 2 && (BlastActor *)gCollisionResults.m_pInstance == victim && gCollisionResults.m_nInstanceNode != -1) {
        shield = &gCollisionResults.m_Frame;
        falloff = 0;
    } else {
        shield = CGameObjectInstance__GetNearestBox(victim, position, &D_800F1970) ? &D_800F1970 : NULL;
    }
    *found = shield;
    if (shielded && shield != NULL) {
        scale *= shield->damageFactor;
    }
    if (falloff != 0) {
        body = victim->pos;
        bottom = victim->pos.y + victim->height;
        top = bottom + GameObject_GetCollisionHeight(victim);
        if (top < position.y) {
            body.y = top;
        } else if (position.y < bottom) {
            body.y = bottom;
        } else {
            body.y = position.y;
        }
        reach = radius + GameObject_GetCollisionRadius(victim);
        distance = CVector3__Distance(&position, &body);
        if (reach < distance) {
            return 0.0f;
        }
        if (scaled) {
            scale *= 1.0f - distance / reach;
        }
    }
    return scale;
}

/* Applies a blast from source to victim: a player's own close-range shot
 * (no weapon, or weapon 0x22) is moved most of the way back to the player;
 * then damage, push and the hit report, scaled by blastScale. */
void ImpactEvent_ApplyBlast(BlastActor *victim, BlastActor *source, s32 hit, CVector3 pos, Blast blast, s32 falloff, s32 push, u32 flags) {
    CVector3 dir;
    DamageEvent info;
    BlastOwner *owner;
    HitFrame *shield;
    f32 mult;
    s32 close;
    f32 radius;
    f32 damage;

    if (Blast_CanHit(victim, source) == 0) {
        return;
    }
    hit = 0;
    if (source->kind == 1 && (source->flags & 0x300000)) {
        owner = source->owner;
        if (source == (BlastActor *) ((u8 *) owner + 0x288) && (owner->weapon < 2 || (s16) owner->weapon == 0x22)) {
            CVector3 from;
            CVector3 to;

            from = pos;
            to = owner->pos;
            CVector3__Lerp(&pos, 0.75f, &from, &to);
            pos.y += GameObject_GetCollisionHeight(owner) * 0.5f;
            hit = 1;
        }
    }
    close = hit;
    radius = blast.radius * 0.04f;
    if (source->kind == 1) {
        mult = CGameObjectInstance__GetAverageScale(source);
    } else {
        mult = 1.0f;
    }
    radius *= mult;
    damage = blast.damage;
    damage *= blastScale(victim, source, pos, radius, falloff, flags, &shield);
    if (damage != 0.0f) {
        Blast_GetPush(victim, source, blast, &dir, flags);
        DamageEvent_Set(&info, source, damage, radius, flags, shield);
        CAI__CallHurt(victim, victim->ai, &info);
        Blast_ApplyHit(victim, source, close, pos, dir, push, shield, flags);
    }
}
