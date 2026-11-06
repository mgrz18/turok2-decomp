/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_2.CPP, func_80214624_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 region;
} Location;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} PlanInfo;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x30 - 8];
    /* 0x30 */ PlanInfo *info;
    /* 0x34 */ u8 pad34[0x68 - 0x34];
    /* 0x68 */ CGameObjectInstance *ignore;
    /* 0x6C */ u8 pad6C[0x80 - 0x6C];
    /* 0x80 */ CGameObjectInstance *approachFrom;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[4];
    /* 0x8C */ f32 rotY;
    /* 0x90 */ f32 height;
    /* 0x94 */ s8 location;
    /* 0x95 */ u8 pad95[0xAC - 0x95];
    /* 0xAC */ CVector3 pos;
    /* 0xB8 */ s32 region;
} Plan;

extern s32 gScene_m_bStreamAll;

Location *CPathTrack__GetCurrentPoint(s8 *location);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a + b */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Normalize(CVector3 *v);                          /* normalise */
f32 CVector3__GetYaw(CVector3 *v);                           /* heading */
f32 GameObject_GetCollisionRadius(CGameObjectInstance *a);                           /* radius */
void GameObject_TraceMove(CGameObjectInstance *a, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 x, s32 y);
s32 Path_FindFleeRegion(CGameObjectInstance *a, s32 b, CVector3 pos, s32 region, s32 d, CVector3 *out);
s32 Path_FindRegionPath(CGameObjectInstance *a, CVector3 pos, s32 region, s32 d, CVector3 *hit);
s32 CGameRegion__ContainsXZ(s32 region, f32 x, f32 z);
f32 AI_AngleToPoint(CGameObjectInstance *a, CVector3 point);

/* The point beside `target`, on the side away from `from`, where `self`
 * stands touching it. */
static __inline__ void Approach(CGameObjectInstance *self, CGameObjectInstance *target, CGameObjectInstance *from, CVector3 *outPos, s32 *outRegion) {
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
    GameObject_TraceMove(target, to, outPos, outRegion, 0, 0);
}

s32 AI_UpdatePathHeading(CGameObjectInstance *self, Plan *plan, CGameObjectInstance *target) {
    CVector3 pos;
    CVector3 hit;
    s32 region;
    s32 mode;
    CPlayer *player;
    Location *loc;

    if (plan->active == 0) {
        mode = 6;
    } else if (target == NULL) {
        mode = 3;
        if (plan->location != 0) {
            mode = 4;
        }
    } else {
        mode = 2;
        if (target != plan->ignore) {
            if (target->header.m_pIntelligence->m_Type == 5) {
                mode = 5;
            } else if (target->m_nTypeFlag == 0x64F) {
                mode = 7;
            } else {
                mode = 0;
                if (gScene_m_bStreamAll == 0) {
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
        region = ((s32)self->header.m_pCurrentRegion);
        break;
    case 4:
        loc = CPathTrack__GetCurrentPoint(&plan->location);
        pos = loc->pos;
        region = loc->region;
        break;
    case 3:
        pos = plan->pos;
        region = plan->region;
        break;
    case 7:
        Approach(self, target, plan->approachFrom, &pos, &region);
        break;
    case 1:
        region = Path_FindFleeRegion(self, plan->unk84, target->header.m_vPos, ((s32)target->header.m_pCurrentRegion), plan->info->unkC, &pos);
        if (region == 0) {
            do {
    default:
                pos = target->header.m_vPos;
                region = ((s32)target->header.m_pCurrentRegion);
            } while (0);
        }
        break;
    }
    if (Path_FindRegionPath(self, pos, region, plan->info->unkC, &hit) != 0) {
        if (CGameRegion__ContainsXZ(((s32)self->header.m_pCurrentRegion), hit.x, hit.z) != 0) {
            plan->height = hit.y;
        } else {
            plan->height = hit.y + 51.199997f;
        }
        CVector3__Subtract(&hit, &hit, &self->header.m_vPos);
        plan->rotY = CVector3__GetYaw(&hit) + 3.1415927f;
        return 1;
    }
    if (mode == 1) {
        plan->height = pos.y;
        plan->rotY = self->m_RotY + AI_AngleToPoint(self, pos);
        return 1;
    }
    return 0;
}
