/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM_2/802661FC_2.CPP, func_802676EC_de (inlined here), with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/collision.h"
#include "globals/player.h"
#include "structs/game_object.h"
#include "structs/player.h"

/* A blast: radius, damage, then the push (strength, x, y, z). */
#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): an impact event's 8-byte value arrives as two
 * host-order words (the event values' convention: CEventValue in structs/particle.h, the
 * animation events' s32[2] from the archive), word 0 the N64's first big-endian word. On the
 * little-endian hosts the halves and bytes of each word sit in reverse order, so the fields
 * that read them by position are declared reversed. (With the N64 order, every particle hit
 * swapped radius and damage: demo 1's first enemy shot took 1 health instead of 8.) */
typedef struct Blast {
    u16 damage;
    u16 radius;
    s8 z;
    s8 y;
    s8 x;
    s8 strength;
} __attribute__((aligned(4))) Blast;
#else
typedef struct Blast {
    u16 radius;
    u16 damage;
    s8 strength;
    s8 x;
    s8 y;
    s8 z;
} __attribute__((aligned(4))) Blast;
#endif



s32 Blast_CanHit(CGameObjectInstance *victim, CGameObjectInstance *source);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
f32 GameObject_GetCollisionHeight(void *actor);
f32 CGameObjectInstance__GetAverageScale(CGameObjectInstance *actor);
s32 CGameObjectInstance__GetNearestBox(CGameObjectInstance *actor, CVector3 pos, HitFrame *out);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *actor);
f32 CVector3__Distance(CVector3 *a, CVector3 *b);
void Blast_GetPush(CGameObjectInstance *victim, CGameObjectInstance *source, Blast blast, CVector3 *out, u32 flags);
void DamageEvent_Set(DamageEvent *out, void *source, s32 damage, f32 radius, u32 flags, HitFrame *shield);
void CAI__CallHurt(CGameObjectInstance *victim, void *ai, DamageEvent *hit);
void Blast_ApplyHit(CGameObjectInstance *victim, CGameObjectInstance *source, s32 hit, CVector3 pos, CVector3 dir, s32 push, HitFrame *shield, u32 flags);

/* How strongly the blast reaches the victim: through its shield, and with
 * falloff over the radius plus its width from the nearest point of its body. */
static __inline__ f32 blastScale(CGameObjectInstance *victim, CGameObjectInstance *source, CVector3 position, f32 radius, s32 falloff, u32 flags, HitFrame **found) {
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
    if (gEngineApp_m_Settings.multiplayer != 0 && (flags & 0x400000)) {
        shielded = 0;
    }
    if (source->header.m_Type == 2 && (CGameObjectInstance *)gCollisionResults.m_pInstance == victim && gCollisionResults.m_nInstanceNode != -1) {
        shield = &gCollisionResults.m_Frame;
        falloff = 0;
    } else {
        shield = CGameObjectInstance__GetNearestBox(victim, position, &sBlastShieldFrame) ? &sBlastShieldFrame : NULL;
    }
    *found = shield;
    if (shielded && shield != NULL) {
        scale *= shield->damageFactor;
    }
    if (falloff != 0) {
        body = victim->header.m_vPos;
        bottom = victim->header.m_vPos.y + victim->m_CollisionYOffset;
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
void ImpactEvent_ApplyBlast(CGameObjectInstance *victim, CGameObjectInstance *source, s32 hit, CVector3 pos, Blast blast, s32 falloff, s32 push, u32 flags) {
    CVector3 dir;
    DamageEvent info;
    CPlayer *owner;
    HitFrame *shield;
    f32 mult;
    s32 close;
    f32 radius;
    f32 damage;

    if (Blast_CanHit(victim, source) == 0) {
        return;
    }
    hit = 0;
    if (source->header.m_Type == 1 && (source->m_dwFlags & 0x300000)) {
        owner = (CPlayer *)source->m_AI.m_pParent;
        /* The blast comes from the player's own weapon actor (round 23: the member, not the N64
         * offset 0x288, which on the host fell inside m_CharacterActor); the weapon is read
         * unsigned, then sign-extended for 0x22. */
        if (source == &owner->m_WeaponActor && ((u16)owner->m_Data.m_CurrentWeapon < 2 || (s16)(u16)owner->m_Data.m_CurrentWeapon == 0x22)) {
            CVector3 from;
            CVector3 to;

            from = pos;
            to = owner->m_CharacterActor.header.m_vPos;
            CVector3__Lerp(&pos, 0.75f, &from, &to);
            pos.y += GameObject_GetCollisionHeight(owner) * 0.5f;
            hit = 1;
        }
    }
    close = hit;
    radius = blast.radius * 0.04f;
    if (source->header.m_Type == 1) {
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
        CAI__CallHurt(victim, &victim->m_AI, &info);
        Blast_ApplyHit(victim, source, close, pos, dir, push, shield, flags);
    }
}
