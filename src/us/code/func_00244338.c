/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_3.CPP, func_80214DD4_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 region;
} Location;

extern s32 D_800F7F78;
extern s32 D_8011001C;

s32 func_0025E428(CGameObjectInstance *self, CAI *plan);
s32 func_002438C0(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target);
CGameObjectInstance *func_00243C90(CGameObjectInstance *self, CAI *plan, s32 arg2, s32 arg3);
f32 func_00244E84(CGameObjectInstance *self, CVector3 pos, CGameObjectInstance *target, f32 scale);
f32 func_00246310(CGameObjectInstance *a, CVector3 point);              /* angle to a point */
Location *func_0028479C(s8 *location);
f32 func_002671B4(CGameObjectInstance *a);                           /* height */
f32 func_002672C8(CGameObjectInstance *a);                           /* radius */
f32 func_00268460(CGameObjectInstance *a);                           /* reach */
f32 func_00268A5C(CGameObjectInstance *a);                           /* ceiling */
void func_002688C8(CGameObjectInstance *a, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 x, s32 y);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a + b */
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020F040(CVector3 *v);                          /* normalise */
f32 func_0020EAA0(CVector3 *v);                           /* heading */
f32 func_002982F0(f32 x);                              /* sin */
f32 func_002974C0(f32 x);                              /* cos */
f32 func_00298470(f32 x);                              /* sqrt */

/* The point beside `target`, on the side away from `from`, where `self`
 * stands touching it. */
static __inline__ void Approach(CGameObjectInstance *self, CGameObjectInstance *target, CGameObjectInstance *from, CVector3 *outPos, s32 *outRegion) {
    CVector3 dir;
    CVector3 to;
    f32 dist;

    if (from != NULL) {
        func_0020EF2C(&dir, &target->header.m_vPos, &from->header.m_vPos);
        dir.y = 0.0f;
        func_0020F040(&dir);
    } else {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
    }
    dist = func_002672C8(target) + func_002672C8(self) + 51.199997f;
    func_0020EF60(&dir, &dir, dist);
    func_0020EEF8(&to, &target->header.m_vPos, &dir);
    func_002688C8(target, to, outPos, outRegion, 0, 0);
}

/* Picks what the plan aims at (its target, itself, a location or home
 * point, or the actor) and fills `out` with the kind, target, range, goal
 * position and its offset from the actor, in 3D and flattened. */
void func_00244338(CGameObjectInstance *self, CAI *plan, s32 boost, CAITarget *out) {
    CVector3 pos;
    CVector3 probe;
    CVector3 away;
    CVector3 delta;
    s32 region;
    CGameObjectInstance *target;
    s32 kind;
    s32 mode;
    f32 range;
    f32 gap;
    f32 angle;
    f32 reach;
    CPlayer *player;
    Location *loc;

    boost = 0; /* FAKEMATCH: the unused third parameter doubles as the boost flag (as in Rage Wars' func_80214DD4_de); a separate local gives the same code but swaps $s5/$s6 in the prologue. */
    if (self->header.m_pIntelligence->m_Type == 1) {
        mode = func_0025E428(self, plan);
    } else {
        mode = -1;
    }
    if (plan->m_PathFindCount == D_800F7F78) {
        if ((plan->m_dwFlags & 0x20000000) && plan->m_pTargetOverride == NULL) {
            if (func_002438C0(self, plan, plan->m_pPathTarget) == 0) {
                plan->m_dwFlags &= ~0x20000000;
            }
        } else {
            plan->m_pPathTarget = func_00243C90(self, plan, 0, 1);
        }
    }
    target = plan->m_pPathTarget;
    if (plan->m_Health == 0) {
        kind = 6;
    } else if (target == NULL) {
        kind = 3;
        if (plan->m_PathTrack.m_Mode != 0) {
            kind = 4;
        }
    } else {
        kind = 2;
        if (target != plan->m_pParent) {
            if (target->header.m_pIntelligence->m_Type == 5) {
                kind = 5;
            } else if (target->m_nTypeFlag == 0x64F) {
                kind = 7;
            } else {
                kind = 0;
                if (D_8011001C == 0) {
                    if ((!(target->m_dwFlags & 0x300000)
                         || (player = ((CPlayer *)target->m_AI.m_pParent),
                             (!(player->m_FleeTime > 0.0f) || (kind = 1, self->m_nTypeFlag == 0xCA && player->m_Data.m_CurrentWeapon == 0x14))
                             && (((CGameObjectInstance *)player->m_pCrossHairTarget) != self || (kind = 1, player->m_CrossHairMode != 2))))
                        && (!(self->m_dwHotPointFlags & 2) || (kind = 1, self->m_nTypeFlag == 0xCA))) {
                        kind = 0;
                    }
                }
            }
        }
    }
    if (target != NULL) {
        if (kind == 7) {
            Approach(self, target, plan->m_pSightTarget, &pos, &region);
        } else {
            pos = target->header.m_vPos;
            region = ((s32)target->header.m_pCurrentRegion);
            if (mode == 1) {
                pos.y += target->m_CollisionYOffset + func_002671B4(target) * 0.9f;
            }
        }
        gap = func_002672C8(target) + func_002672C8(self);
        switch (kind) {
        case 2:
            gap += 102.399994f;
            break;
        case 5:
        case 7:
            gap = 0.0f;
            break;
        }
        boost = 1;
        if (plan->m_PathTrack.m_Mode == 1) {
            plan->m_PathTrack.m_Mode = 2;
        } else if (plan->m_PathTrack.m_Mode == 3) {
            plan->m_PathTrack.m_Mode = 4;
        }
    } else {
        gap = func_002672C8(self);
        switch (kind) {
        case 4:
            loc = func_0028479C(&plan->m_PathTrack.m_Mode);
            pos = loc->pos;
            gap = 0.0f;
            region = loc->region;
            if (plan->m_PathTrack.m_Mode == 2 || plan->m_PathTrack.m_Mode == kind) {
                boost = 1;
            }
            break;
        case 3:
            pos = plan->m_vLeashPos;
            region = plan->m_pLeashRegion;
            boost = 1;
            break;
        case 6:
            pos = self->header.m_vPos;
            region = ((s32)self->header.m_pCurrentRegion);
            if (mode == 2) {
                pos.y = func_00268A5C(self);
            }
            break;
        }
    }
    func_0020EEF8(&pos, &pos, &plan->m_vTargetOffset);
    if (gap > 0.0f) {
        func_0020EF2C(&away, &self->header.m_vPos, &pos);
        angle = func_0020EAA0(&away);
        pos.x -= gap * func_002982F0(angle);
        pos.z -= gap * func_002974C0(angle);
    }
    if (mode == 2) {
        pos.y = (pos.y < func_00268A5C(self)) ? pos.y : func_00268A5C(self);
    }
    if (boost) {
        reach = func_00268460(self);
        probe.x = self->header.m_vPos.x + reach * func_002982F0(plan->m_PathRotY);
        probe.y = plan->m_PathHeight;
        probe.z = self->header.m_vPos.z + reach * func_002974C0(plan->m_PathRotY);
        range = func_00244E84(self, probe, target, 1.5f);
        pos.y = plan->m_PathHeight;
    } else {
        range = func_00246310(self, pos);
    }
    func_0020EF2C(&delta, &pos, &self->header.m_vPos);
    out->m_Kind = kind;
    out->m_pTarget = target;
    out->m_Angle = range;
    out->m_vPos = pos;
    out->m_vDelta = delta;
    out->m_Dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0.0f;
    out->m_vFlatPos = pos;
    out->m_vFlatDelta = delta;
    out->m_FlatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}
