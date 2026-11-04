/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00215410.c, func_80215410_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 region;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ u16 flags;
} Location;

extern s32 D_800F7F78;
extern s32 D_8011001C;

CGameObjectInstance *func_00243C90(CGameObjectInstance *self, CAI *plan, s32 arg2, s32 arg3);
Location *func_0028479C(s8 *location);
f32 func_00246310(CGameObjectInstance *a, CVector3 point);              /* angle to a point */
void func_00210EF0(f32 *angle);                        /* wrap an angle */
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
f32 func_00298470(f32 x);                              /* sqrt */

/* What the plan aims at (the same classes as func_002438C0). */
static __inline__ s32 Kind(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target) {
    CPlayer *player;

    if (plan->m_Health == 0) {
        return 6;
    }
    if (target == NULL) {
        if (plan->m_PathTrack.m_Mode != 0) {
            return 4;
        }
        return 3;
    }
    if (target == plan->m_pParent) {
        return 2;
    }
    if (target->header.m_pIntelligence->m_Type == 5) {
        return 5;
    }
    if (target->m_nTypeFlag == 0x64F) {
        return 7;
    }
    if (D_8011001C == 0) {
        if (target->m_dwFlags & 0x300000) {
            player = ((CPlayer *)target->m_AI.m_pParent);
            if (player->m_FleeTime > 0.0f && (self->m_nTypeFlag != 0xCA || player->m_Data.m_CurrentWeapon != 0x14)) {
                return 1;
            }
            if (((CGameObjectInstance *)player->m_pCrossHairTarget) == self && player->m_CrossHairMode == 2) {
                return 1;
            }
        }
        if ((self->m_dwHotPointFlags & 2) && self->m_nTypeFlag != 0xCA) {
            return 1;
        }
    }
    return 0;
}

/* Picks the plan's target and fills `out` with its kind, the turn to face
 * it, its position, offset and distance, in 3D and flattened. */
void func_002449E0(CGameObjectInstance *self, CAI *plan, s32 unused, CAITarget *out) {
    CVector3 pos;
    CVector3 delta;
    f32 angle;
    s32 kind;
    CGameObjectInstance *target;
    Location *loc;

    if (plan->m_PathFindCount == D_800F7F78 && (!(plan->m_dwFlags & 0x40000000) || plan->m_pTargetOverride != NULL)) {
        plan->m_pSightTarget = func_00243C90(self, plan, 1, 0);
    }
    if (self->header.m_pIntelligence->m_Type == 1 && plan->m_Mode == 0xB && Kind(self, plan, plan->m_pPathTarget) == 4) {
        target = plan->m_pPathTarget;
        kind = 4;
    } else {
        target = plan->m_pSightTarget;
        kind = Kind(self, plan, target);
    }
    if (target != NULL) {
        pos = target->header.m_vPos;
        angle = func_00246310(self, pos);
    } else {
        switch (kind) {
        case 4:
            loc = func_0028479C(&plan->m_PathTrack.m_Mode);
            pos = loc->pos;
            if (loc->flags & 1) {
                angle = loc->rotY - self->m_RotY;
            } else {
                angle = func_00246310(self, pos);
            }
            break;
        case 3:
            pos = plan->m_vLeashPos;
            angle = plan->m_InitialRotY - self->m_RotY;
            break;
        case 6:
            pos = self->header.m_vPos;
            angle = 0.0f;
            break;
        }
    }
    func_00210EF0(&angle);
    func_0020EF2C(&delta, &pos, &self->header.m_vPos);
    out->m_Kind = kind;
    out->m_pTarget = target;
    out->m_Angle = angle;
    out->m_vPos = pos;
    out->m_vDelta = delta;
    out->m_Dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0;
    out->m_vFlatPos = pos;
    out->m_vFlatDelta = delta;
    out->m_FlatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}
