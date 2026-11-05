#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"


void func_002470E8(void *controls, void *settings);
f32 func_00253CFC(CPlayer *player);
void func_0024E5F0(CPlayer *player, CPlayer *arg1, s32 arg2);

/* The ROM clears the float fields with integer stores (sw $zero): a 0.0f
 * store keeps the constant in a register instead. */
#define CLEAR_WORD(field) (*(s32 *) &(field) = 0)

static __inline__ void setVec3(CVector3 *v, f32 x, f32 y, f32 z) {
    v->z = z;
    v->y = y;
    v->x = x;
}

/* Resets a player's movement, camera and crosshair state. */
void func_00248340(CPlayer *p) {
    f32 zero;

    func_002470E8(&p->m_ContState, &D_800F5EC0[p->m_nController]);
    CLEAR_WORD(p->m_Speed);
    CLEAR_WORD(p->m_SideSpeed);
    CLEAR_WORD(p->m_TotalSpeed);
    CLEAR_WORD(p->m_YVelLastFrame);
    p->m_bOnGround = 1;
    CLEAR_WORD(p->m_BurstSpeed);
    CLEAR_WORD(p->m_BurstSideSpeed);
    CLEAR_WORD(p->m_BurstTimer);
    CLEAR_WORD(p->m_TimeOffGround);
    CLEAR_WORD(p->m_BreathAmp);
    CLEAR_WORD(p->m_BreathSpeed);
    p->m_bHeadMovementDisabled = 0;
    CLEAR_WORD(p->m_JumpAngle);
    CLEAR_WORD(p->m_StartJumpAngle);
    CLEAR_WORD(p->m_DesiredJumpAngle);
    CLEAR_WORD(p->m_JumpU);
    CLEAR_WORD(p->m_vHeadRotOffset.x);
    CLEAR_WORD(p->m_vHeadRotOffset.y);
    CLEAR_WORD(p->m_vHeadRotOffset.z);
    CLEAR_WORD(p->m_vHeadWobbleRotOffset.x);
    CLEAR_WORD(p->m_vHeadWobbleRotOffset.y);
    CLEAR_WORD(p->m_vHeadWobbleRotOffset.z);
    CLEAR_WORD(p->m_vEyeOffset.x);
    p->m_vEyeOffset.y = func_00253CFC(p);
    CLEAR_WORD(p->m_vEyeOffset.z);
    setVec3((CVector3 *) &p->m_qGround, 0.0f, 0.0f, 0.0f);
    CLEAR_WORD(p->m_ActualCameraVertEyeOffset);
    p->m_qGround.w = 1.0f;
    p->m_CameraMovementScaler = 1.0f;
    p->m_vDesiredPos = p->m_CharacterActor.header.m_vPos;
    CLEAR_WORD(p->m_vCrossHairVel.z);
    zero = p->m_vCrossHairVel.z;   /* read back as a float: the ROM reloads it */
    CLEAR_WORD(p->m_ModeTime);
    p->m_InSun = 0;
    CLEAR_WORD(p->m_FrameIncrementScaled);
    CLEAR_WORD(p->m_DuckOffset);
    p->m_bDuckMode = 0;
    CLEAR_WORD(p->m_ZoomBlend);
    CLEAR_WORD(p->m_TargetZoomBlend);
    p->m_SniperMode = 0;
    p->m_CrossHairMode = 0;
    CLEAR_WORD(p->m_CrossHairTimer);
    CLEAR_WORD(p->m_CrossHairAlpha);
    p->m_pCrossHairTarget = NULL;
    p->m_CrossHairIcon = -1;
    CLEAR_WORD(p->m_vCrossHairPos.x);
    CLEAR_WORD(p->m_vCrossHairPos.y);
    CLEAR_WORD(p->m_vCrossHairPos.z);
    CLEAR_WORD(p->m_vCrossHairVel.x);
    CLEAR_WORD(p->m_vCrossHairVel.y);
    setVec3((CVector3 *) &p->m_qAutoAim, zero, zero, zero);
    p->m_qAutoAim.w = 1.0f;
    p->m_pSightTarget = NULL;
    p->m_SightIcon = -1;
    p->m_vSightPos.x = zero;
    p->m_vSightPos.y = zero;
    p->m_vSightPos.z = zero;
    p->m_vSightVel.x = zero;
    p->m_vSightVel.y = zero;
    p->m_vSightVel.z = zero;
    func_0024E5F0(p, p, 2);
    p->m_pOnEnemy = NULL;
    p->m_RideEnemyTime = zero;
    p->m_OnEnemyStompTime = zero;
    p->m_Oxygen = zero;
    p->m_bOxygenOut = 0;
    p->m_pOptions->m_bDead = 0;
    p->m_FleeTime = zero;
}
