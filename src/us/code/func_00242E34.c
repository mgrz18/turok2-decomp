#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/intelligence.h"
#include "structs/collision.h"
#include "structs/player.h"
#include "structs/camera.h"


typedef struct Dest {
    u32 flags;         /* 0x00 */
    char pad4[0xCC - 0x4];
    f32 timer;         /* 0xCC */
} Dest;

extern char gScene[];
extern f32 gFrameIncrement;  /* frame time scale */

CRegionInfo *CScene__GetRegionInfo(void *world, CGameRegion *region);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);
f32 GameObject_GetWaterHeight(CGameObjectInstance *actor);
s32 CGameRegion__IsSteep(CGameRegion *region);
s32 func_0021F080(CGameObjectInstance *actor);
f32 CGameRegion__GetHeight(CGameRegion *region, f32 x, f32 z);
s32 GameObject_IsOnGround(CGameObjectInstance *actor);
void DamageEvent_Set(DamageEvent *damage, s32 arg1, s32 amount, f32 force, s32 kind, s32 arg5);
void CAI__CallHurt(CGameObjectInstance *actor, Dest *dst, DamageEvent *damage);

/* CGameRegion flags for the actor (water, ground...) and periodic damage from the region it stands in. */
void AI_UpdateRegionFlags(CGameObjectInstance *actor, Dest *dst) {
    CGameRegion *region;
    CRegionInfo *info;
    DamageEvent damage;
    f32 y;
    f32 top;
    s32 hurt;
    s32 kind;

    dst->flags &= ~0x7F80;
    region = actor->header.m_pCurrentRegion;
    if (region == NULL) {
        return;
    }
    info = CScene__GetRegionInfo(gScene, region);
    if (info == NULL) {
        return;
    }
    if (info->m_dwFlags & 0x1000000) {
        dst->flags |= 0x800;
    }
    if (region->m_wFlags & 1) {
        if (info->m_dwFlags & 0x20000000) {
            dst->flags |= 0x400;
        } else {
            if (actor->m_dwFlags & 0x300000) {
                y = actor->header.m_vPos.y;
                top = ((CPlayer *) actor->m_AI.m_pParent)->m_pCamera->m_vTCorners[0].y;
            } else {
                top = GameObject_GetCollisionHeight(actor) + actor->header.m_vPos.y;
                y = actor->header.m_vPos.y;
            }
            if (top <= GameObject_GetWaterHeight(actor)) {
                dst->flags |= 0x80;
            } else if (GameObject_GetWaterHeight(actor) < y) {
                dst->flags |= 0x100;
            } else {
                dst->flags |= 0x200;
            }
        }
    }
    if (CGameRegion__IsSteep(region) && (region->m_wFlags & 0x10) && func_0021F080(actor)) {
        dst->flags |= 0x1000;
    }
    if (CGameRegion__GetHeight(region, actor->header.m_vPos.x, actor->header.m_vPos.z) - actor->header.m_vPos.y < 15.36f) {
        if (region->m_wFlags & 0x100) {
            dst->flags |= 0x2000;
        }
        if (region->m_wFlags & 0x80) {
            dst->flags |= 0x4000;
        }
    }
    if (info->m_dwFlags & 0x80000) {
        hurt = 1;
        if (info->m_dwFlags & 4) {
            hurt = actor->header.m_pIntelligence->m_Type != 1;
        }
        if (dst->flags & 0xC0000) {
            hurt = 0;
        }
        if (!GameObject_IsOnGround(actor)) {
            hurt = 0;
        }
        if (actor->m_pInstanceBelow != NULL) {
            hurt = 0;
        }
        if (hurt) {
            if (!(dst->flags & 0x8000)) {
                dst->timer += gFrameIncrement * (1.0f / 15);
                if (dst->timer >= info->m_DamageInterval) {
                    kind = 0x100000;
                    if (info->m_dwFlags & 0x400000) {
                        kind = 0x1100000;
                    }
                    dst->timer -= info->m_DamageInterval;
                    DamageEvent_Set(&damage, 0, info->m_DamageAmount << 8, 25.599998f, kind, 0);
                    CAI__CallHurt(actor, dst, &damage);
                }
            }
            if (dst->flags & 0x8000) {
                dst->timer = info->m_DamageInterval;
                dst->flags &= ~0x8000;
            }
        } else {
            dst->flags &= ~0x8000;
        }
    } else {
        dst->flags |= 0x8000;
    }
}
