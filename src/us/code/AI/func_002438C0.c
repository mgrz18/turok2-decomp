/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_2.CPP, func_80214624_de, with Turok 2's layouts. */
#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "globals/scene.h"
#ifdef TARGET_PORT
sptr CScene__GetPathRegion(s32 ref);
#endif






sptr Path_FindFleeRegion(CGameObjectInstance *a, sptr b, CVector3 pos, sptr region, sptr d, CVector3 *out);
s32 Path_FindRegionPath(CGameObjectInstance *a, CVector3 pos, sptr region, sptr d, CVector3 *hit);
s32 CGameRegion__ContainsXZ(sptr region, f32 x, f32 z);

/* The point beside `target`, on the side away from `from`, where `self`
 * stands touching it. */
static __inline__ void Approach(CGameObjectInstance *self, CGameObjectInstance *target, CGameObjectInstance *from, CVector3 *outPos, sptr *outRegion) {
    CVector3 dir;
    CVector3 to;
    f32 dist;

    if (from != NULL) {
        CVector3__Subtract(&dir, &target->header.m_vPos, &from->header.m_vPos);
        dir.y = 0.0f;
        CVector3__Normalize(&dir);
    } else {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
    }
    dist = GameObject_GetCollisionRadius(target) + GameObject_GetCollisionRadius(self) + 51.199997f;
    CVector3__Scale(&dir, &dir, dist);
    CVector3__Add(&to, &target->header.m_vPos, &dir);
    GameObject_TraceMove(&target->header, to, outPos, (CGameRegion **) outRegion, 0, 0);
}

s32 AI_UpdatePathHeading(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target) {
    CVector3 pos;
    CVector3 hit;
    sptr region;
    s32 mode;
    CPlayer *player;
    CPathPoint *loc;

    if (plan->m_Health == 0) {
        mode = 6;
    } else if (target == NULL) {
        mode = 3;
        if (plan->m_PathTrack.m_Mode != 0) {
            mode = 4;
        }
    } else {
        mode = 2;
        if (target != plan->m_pParent) {
            if (target->header.m_pIntelligence->m_Type == 5) {
                mode = 5;
            } else if (target->m_nTypeFlag == 0x64F) {
                mode = 7;
            } else {
                mode = 0;
                if (gScene.m_bStreamAll == 0) {
                    if ((!(target->m_dwFlags & 0x300000)
                         || (player = ((CPlayer *)target->m_AI.m_pParent),
                             (!(player->m_FleeTime > 0.0f) || (mode = 1, self->m_nTypeFlag == 0xCA && player->m_Data.m_CurrentWeapon == 0x14))
                             && (((CGameObjectInstance *)player->m_pCrossHairTarget) != self || (mode = 1, player->m_CrossHairMode != 2))))
                        && (!(self->m_dwHotPointFlags & 2) || (mode = 1, self->m_nTypeFlag == 0xCA))) {
                        mode = 0;
                    }
                }
            }
        }
    }
    switch (mode) {
    case 6:
        pos = self->header.m_vPos;
        region = ((sptr)self->header.m_pCurrentRegion);
        break;
    case 4:
        loc = CPathTrack__GetCurrentPoint(&plan->m_PathTrack);
        pos = loc->m_vPos;
#ifdef TARGET_PORT
        region = CScene__GetPathRegion(loc->m_Region); /* the region index (func_0042CD04.c) */
#else
        region = loc->m_Region;
#endif
        break;
    case 3:
        pos = plan->m_vLeashPos;
        region = (sptr) plan->m_pLeashRegion;
        break;
    case 7:
        Approach(self, target, plan->m_pSightTarget, &pos, &region);
        break;
    case 1:
        region = Path_FindFleeRegion(self, (sptr) plan->m_pPathAvoidRegion, target->header.m_vPos, ((sptr)target->header.m_pCurrentRegion), (sptr) plan->m_pModeCurrent->m_pData, &pos);
        if (region == 0) {
            do {
    default:
                pos = target->header.m_vPos;
                region = ((sptr)target->header.m_pCurrentRegion);
            } while (0);
        }
        break;
    }
    if (Path_FindRegionPath(self, pos, region, (sptr) plan->m_pModeCurrent->m_pData, &hit) != 0) {
        if (CGameRegion__ContainsXZ(((sptr)self->header.m_pCurrentRegion), hit.x, hit.z) != 0) {
            plan->m_PathHeight = hit.y;
        } else {
            plan->m_PathHeight = hit.y + 51.199997f;
        }
        CVector3__Subtract(&hit, &hit, &self->header.m_vPos);
        plan->m_PathRotY = CVector3__GetYaw(&hit) + 3.1415927f;
        return 1;
    }
    if (mode == 1) {
        plan->m_PathHeight = pos.y;
        plan->m_PathRotY = self->m_RotY + AI_AngleToPoint(self, pos);
        return 1;
    }
    return 0;
}
