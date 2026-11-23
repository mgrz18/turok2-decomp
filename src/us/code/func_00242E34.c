#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/intelligence.h"
#include "structs/collision.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"


f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);
f32 GameObject_GetWaterHeight(CGameObjectInstance *actor);
s32 CGameRegion__IsSteep(CGameRegion *region);
s32 func_0021F080(CGameObjectInstance *actor);
f32 CGameRegion__GetHeight(CGameRegion *region, f32 x, f32 z);
s32 GameObject_IsOnGround(CGameObjectInstance *actor);
void DamageEvent_Set(DamageEvent *damage, sptr arg1, s32 amount, f32 force, s32 kind, sptr arg5);
void CAI__CallHurt(CGameObjectInstance *actor, CAI *dst, DamageEvent *damage);

/* CGameRegion flags for the actor (water, ground...) and periodic damage from the region it stands in. */
void AI_UpdateRegionFlags(CGameObjectInstance *actor, CAI *dst) {
    CGameRegion *region;
    CRegionInfo *info;
    DamageEvent damage;
    f32 y;
    f32 top;
    s32 hurt;
    s32 kind;

    dst->m_dwFlags &= ~0x7F80;
    region = actor->header.m_pCurrentRegion;
    if (region == NULL) {
        return;
    }
    info = CScene__GetRegionInfo(&gScene, region);
    if (info == NULL) {
        return;
    }
    if (info->m_dwFlags & 0x1000000) {
        dst->m_dwFlags |= 0x800;
    }
    if (region->m_wFlags & 1) {
        if (info->m_dwFlags & 0x20000000) {
            dst->m_dwFlags |= 0x400;
        } else {
            if (actor->m_dwFlags & 0x300000) {
                y = actor->header.m_vPos.y;
                top = ((CPlayer *) actor->m_AI.m_pParent)->m_pCamera->m_vTCorners[0].y;
            } else {
                top = GameObject_GetCollisionHeight(actor) + actor->header.m_vPos.y;
                y = actor->header.m_vPos.y;
            }
            if (top <= GameObject_GetWaterHeight(actor)) {
                dst->m_dwFlags |= 0x80;
            } else if (GameObject_GetWaterHeight(actor) < y) {
                dst->m_dwFlags |= 0x100;
            } else {
                dst->m_dwFlags |= 0x200;
            }
        }
    }
    if (CGameRegion__IsSteep(region) && (region->m_wFlags & 0x10) && func_0021F080(actor)) {
        dst->m_dwFlags |= 0x1000;
    }
    if (CGameRegion__GetHeight(region, actor->header.m_vPos.x, actor->header.m_vPos.z) - actor->header.m_vPos.y < 15.36f) {
        if (region->m_wFlags & 0x100) {
            dst->m_dwFlags |= 0x2000;
        }
        if (region->m_wFlags & 0x80) {
            dst->m_dwFlags |= 0x4000;
        }
    }
    if (info->m_dwFlags & 0x80000) {
        hurt = 1;
        if (info->m_dwFlags & 4) {
            hurt = actor->header.m_pIntelligence->m_Type != 1;
        }
        if (dst->m_dwFlags & 0xC0000) {
            hurt = 0;
        }
        if (!GameObject_IsOnGround(actor)) {
            hurt = 0;
        }
        if (actor->m_pInstanceBelow != NULL) {
            hurt = 0;
        }
        if (hurt) {
            if (!(dst->m_dwFlags & 0x8000)) {
                dst->m_DeathRegionTimer += gFrameIncrement * (1.0f / 15);
                if (dst->m_DeathRegionTimer >= info->m_DamageInterval) {
                    kind = 0x100000;
                    if (info->m_dwFlags & 0x400000) {
                        kind = 0x1100000;
                    }
                    dst->m_DeathRegionTimer -= info->m_DamageInterval;
                    DamageEvent_Set(&damage, 0, info->m_DamageAmount << 8, 25.599998f, kind, 0);
                    CAI__CallHurt(actor, dst, &damage);
                }
            }
            if (dst->m_dwFlags & 0x8000) {
                dst->m_DeathRegionTimer = info->m_DamageInterval;
                dst->m_dwFlags &= ~0x8000;
            }
        } else {
            dst->m_dwFlags &= ~0x8000;
        }
    } else {
        dst->m_dwFlags |= 0x8000;
    }
}
