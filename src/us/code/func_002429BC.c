/* FAKEMATCH: the shared -1 is pinned to $v1 (register asm, see the body): local-alloc gives it $v0, which keeps
 * sched2 from hoisting the load of ai->m_dwFlags above the stores. With the real CAI / CGameObjectInstance
 * fields (k4 pass) the unpinned forms still miss by 9 words: `= -1` in each store, a chained assignment
 * (either order), an int `none`, a loop over m_FlinchNode (61 words), and `ai->m_dwFlags &= ~1` moved to each
 * point between m_AnimIndex and m_AnimSpeed (24 words before the -1 stores, 11-9 after). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/80212C90_18.CPP, ACTOR_SETUP, with Turok 2's layouts. */

#include "common.h"
#include "structs/game_object.h"

typedef struct CAISetupCallbacks {
    void (*first)(CGameObjectInstance *, CAI *);
} CAISetupCallbacks;

void memset(void *dst, s32 value, sptr size); /* memset */
void AI_InitHealth(CAI *ai, CIntelligence *def, CVector3 pos, CGameObjectInstance *obj);
f32 CGameRegion__GetHeight(struct CGameRegion *region, f32 x, f32 z);  /* floor height at */
s32 CGameObjectInstance__DrawModel();
void CGameObjectInstance__CalculateOrientationMatrix();

/* Builds an object's AI: clears it (keeping m_pParent), runs the shared
 * initialiser, installs the two callbacks, seeds the position and heading
 * copies, the flinch nodes and the default speeds, runs the optional first
 * callback, and flags it when the object asks (bit 27). */
void CAI__Setup(CGameObjectInstance *obj, CAI *ai, CAISetupCallbacks *callbacks) {
    struct CGameObjectInstance *parent;
    f32 speed;
    struct CGameRegion *region;
    /* The shared -1 is pinned to $v1: GCC's local allocator always gives
     * it $v0 (it outranks the flags temporary), which stops sched2 from
     * hoisting the load of ai->m_dwFlags above these stores. */
    register s16 none asm("$3");

    parent = ai->m_pParent;
    memset(ai, 0, sizeof(CAI));
    ai->m_dwFlags = 0x40;
    ai->m_pParent = parent;
    AI_InitHealth(ai, obj->header.m_pIntelligence, obj->header.m_vPos, obj);
    ai->m_pfCalculateOrientationMatrix = CGameObjectInstance__CalculateOrientationMatrix;
    ai->m_pfDraw = CGameObjectInstance__DrawModel;
    ai->m_Mode = -1;
    ai->m_ModeLastFrame = -1;
    ai->m_pModeCurrent = NULL;
    ai->m_vPosLastFrame = obj->header.m_vPos;
    ai->m_vDesiredPos = obj->header.m_vPos;
    ai->m_vTargetOffset.x = 0;
    ai->m_vTargetOffset.y = 0;
    ai->m_vTargetOffset.z = 0;
    ai->m_RotYLastFrame = obj->m_RotY;
    ai->m_ConditionTimer = 0;
    ai->m_pTargetOverride = NULL;
    ai->m_TargetOverrideTime = 0;
    ai->m_pSightTarget = NULL;
    ai->m_pPathAvoidRegion = NULL;
    ai->m_pPathTarget = NULL;
    ai->m_PathRotY = obj->m_RotY;
    ai->m_PathHeight = obj->header.m_vPos.y;
    ai->m_InitialRotY = obj->m_RotY;
    ai->m_vInitialPos = obj->header.m_vPos;
    ai->m_pInitialRegion = obj->header.m_pCurrentRegion;
    ai->m_vLeashPos = obj->header.m_vPos;
    region = obj->header.m_pCurrentRegion;
    ai->m_pLeashRegion = region;
    if ((obj->header.m_pIntelligence->m_dwFlags & 4) && region != NULL) {
        ai->m_vLeashPos.y = CGameRegion__GetHeight(region, ai->m_vLeashPos.x, ai->m_vLeashPos.z);
    }
    speed = 1.0f;
    ai->m_AnimIndex = -1;
    ai->m_FlinchTime = 0x14;
    ai->m_PathTrack.m_nCurrentPathIndex = 0xFF;
    none = -1;
    ai->m_AnimType = none;
    ai->m_ModeLastFrame = none;
    ai->m_FlinchNode[0] = none;
    ai->m_FlinchNode[1] = none;
    ai->m_FlinchNode[2] = none;
    ai->m_FlinchNode[3] = none;
    ai->m_PathTrack.m_nCurrentPointIndex = none;
    ai->m_AnimRepeatedCount = 0;
    ai->m_DeathRegionTimer = 0;
    ai->m_OnFireTimer = 0;
    ai->m_ActionTime = 0;
    ai->m_FlinchCountdown = 0;
    ai->m_vFlinchAxis.y = 0;
    ai->m_vFlinchAxis.z = 0;
    ai->m_HeadTurn = 0;
    ai->m_HeadTilt = 0;
    ai->m_PathTrack.m_Mode = 0;
    ai->m_pAttachedLoopingSound = NULL;
    ai->m_AnimSpeed = speed;
    ai->m_AnimModeSpeed = speed;
    ai->m_dwFlags &= ~1;
    ai->m_vFlinchAxis.x = speed;
    if (callbacks != NULL && callbacks->first != NULL) {
        callbacks->first(obj, ai);
    }
    if (obj->m_dwFlags & 0x8000000) {
        ai->m_AnimRepeatedCount = 1;
    }
}
