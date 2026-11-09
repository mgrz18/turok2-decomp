/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM_2/802661FC_2.CPP, func_80267958_de (inlined here), with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "globals/scene.h"
#include "protos/game.h"

/* Applies a hit from an attacker to a target: marks the attacker, shakes the
 * target player's camera for heavy and medium hits, sprays blood from a
 * player attacker for blade kinds 4 and 5, reports it, and pushes the target
 * along the hit direction. */

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 offset;
} PlayerCamera;


typedef struct {
    /* 0x00 */ u8 pad0[0x66];
    /* 0x66 */ u8 kind;
} HitDef;


extern void CCamera__SetShake(PlayerCamera *camera, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos);
extern void CPlayer__TransformByView(CPlayer *player, CVector3 *v);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
extern void GameObject_TraceMove(CGameObjectInstance *actor, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 flagA, s32 flagB);
extern s32 CParticleSystem__Spawn(CParticleSystem *table, void *actor, void *parent, void *slot, s32 a, s32 kind, CVector3 origin,
                         CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);
extern void CScene__StartRumbles(void *table, void *owner, s32 id, CVector3 pos, f32 a, f32 b);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a + b */
extern f32 AI_AngleToPoint(CGameObjectInstance *actor, CVector3 point);

/* Its unused target and def parameters still take stack slots when inlined (the frame layout needs them). */
static __inline__ void sprayBlood(CGameObjectInstance *target, CGameObjectInstance *actor, CVector3 position, CVector3 direction, HitDef *def, s32 kind) {
    CPlayer *player;
    CVector3 v;
    CVector3 point;
    CVector3 zero;
    CQuaternion rotation;
    s32 region;

    player = ((CPlayer *)actor->m_AI.m_pParent);
    v.x = -direction.x;
    v.y = direction.y;
    v.z = direction.z;
    CPlayer__TransformByView(player, &v);
    v.x -= ((PlayerCamera *)player->m_pCamera)->offset.x;
    v.y -= ((PlayerCamera *)player->m_pCamera)->offset.y;
    v.z -= ((PlayerCamera *)player->m_pCamera)->offset.z;
    CQuaternion__FromDirection(&rotation, &v);
    GameObject_TraceMove(actor, position, &point, &region, 0, 1);
    point = position;
    point.y += 12.0f;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor, 0, 0, 0xE9, zero, rotation, point, region, -1, kind == 5 ? 3 : 1);
    point.x = 0;
    point.y = 0;
    point.z = 0;
    CScene__StartRumbles(&gScene, player->m_ContState.m_pContState->rumblePool, 3, point, 10.24f, 1.0f);
}

void Blast_ApplyHit(CGameObjectInstance *target, CGameObjectInstance *source, s32 hit, CVector3 pos, CVector3 dir, s32 push, HitDef *def, u32 flags) {
    CGameObjectInstance *attacker;
    u32 bits;
    CPlayer *player;

    if (source->header.m_Type == 1) {
        source->m_AI.m_dwFlags |= 0x4000000;
        attacker = source;
        if (target->m_dwFlags & 0x300000) {
            bits = source->m_AI.m_ModeFlags;
            player = ((CPlayer *)target->m_AI.m_pParent);
            if (bits & 0x4000) {
                CCamera__SetShake(((PlayerCamera *)player->m_pCamera), 20.0f, 100.0f, 20.0f, 1.0f, 1, pos);
            } else if (bits & 0x2000) {
                CCamera__SetShake(((PlayerCamera *)player->m_pCamera), 10.0f, 50.0f, 10.0f, 1.0f, 1, pos);
            }
        }
        if (hit) {
            if (def != 0 && (attacker->m_dwFlags & 0x300000) && (def->kind == 4 || def->kind == 5)) {
                sprayBlood(target, source, pos, dir, def, def->kind);
            }
            dir.x = 0;
            dir.y = 0;
            dir.z = 0;
        }
        if ((attacker->m_dwFlags & 0x300000) && (flags & 0x400000)) {
            GameObject_FireTriggers(target, 0x10, attacker);
        }
    }
    if (target->header.m_pIntelligence->m_Type == 1 || target->header.m_pIntelligence->m_Type == 11) {
        CVector3__Add(&target->m_vVelocity, &target->m_vVelocity, &dir);
        if (push) {
            target->m_RotY += AI_AngleToPoint(target, source->header.m_vPos) * 0.5f;
        }
    }
}
