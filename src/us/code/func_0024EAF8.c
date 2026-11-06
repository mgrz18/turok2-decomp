#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/collision.h"
#include "globals/player.h"

/* The creature to mount (an AI actor). */
typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x50 - 0x10];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0xB8 - 0x54];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0x140 - 0xBA];
    /* 0x140 */ u8 ai[4];
} Mount;

extern CCollisionResults gCollisionResults;  /* m_pInstance: what the player is aiming at */

f32 AI_AngleToPoint(CGameObjectInstance *body, CVector3 point);   /* angle to a point */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);
void CPlayerContState__Init(void *controls, void *settings);
f32 Actor_GetEyeHeight(CPlayer *player);
void CCache__SetFrameDelay(s32);
void EnemyAI_Mount(Mount *mount, void *ai, CPlayer *player);
s32 CPlayer__SetState(CPlayer *player, CGameObjectInstance *body, s32 arg2);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

static __inline__ void setVec3(CVector3 *v, f32 x, f32 y, f32 z) {
    v->z = z;
    v->y = y;
    v->x = x;
}

/* Mounts the creature of type 0x136 in front, if within 45 degrees of the
 * heading and the player is not holding 0x8000: the player takes its seat
 * and heading, and the movement state is reset. */
s32 CPlayer__TryMount(CPlayer *p, CGameObjectInstance *body) {
    Mount *mount;

    if (p->m_pOnEnemy == NULL && !(p->m_ModeBeforeFlags & 0x8000)) {
        if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
            mount = (Mount *)gCollisionResults.m_pInstance;
            if (mount->type == 0x136 && ABS(AI_AngleToPoint(body, mount->pos)) < 0.785398245f) {
                p->m_pOnEnemy = mount;
                p->m_OnEnemyObjectType = 0x136;
                p->m_vOnEnemyScale = gRidingScale;
                CVector3__Scale(&p->m_vOnEnemyScale, &p->m_vOnEnemyScale, 0.03f);
                body->m_RotY = mount->yaw;
                CPlayerContState__Init(&p->m_ContState, &gControllers[p->m_nController]);
                p->m_Speed = 0;
                p->m_SideSpeed = 0;
                p->m_TotalSpeed = 0;
                p->m_YVelLastFrame = 0;
                p->m_bOnGround = 1;
                p->m_BurstSpeed = 0;
                p->m_BurstSideSpeed = 0;
                p->m_BurstTimer = 0;
                p->m_TimeOffGround = 0;
                p->m_BreathAmp = 0;
                p->m_BreathSpeed = 0;
                p->m_bHeadMovementDisabled = 0;
                p->m_JumpAngle = 0;
                p->m_StartJumpAngle = 0;
                p->m_DesiredJumpAngle = 0;
                p->m_JumpU = 0;
                p->m_vHeadRotOffset.x = 0;
                p->m_vHeadRotOffset.y = 0;
                p->m_vHeadRotOffset.z = 0;
                p->m_vHeadWobbleRotOffset.x = 0;
                p->m_vHeadWobbleRotOffset.y = 0;
                p->m_vHeadWobbleRotOffset.z = 0;
                p->m_vEyeOffset.x = 0;
                p->m_vEyeOffset.y = Actor_GetEyeHeight(p);
                p->m_vEyeOffset.z = 0;
                setVec3(&p->m_qGround, 0.0f, 0.0f, 0.0f);
                p->m_ActualCameraVertEyeOffset = 0;
                p->m_qGround.w = 1.0f;
                p->m_CameraMovementScaler = 1.0f;
                p->m_vDesiredPos = p->m_CharacterActor.header.m_vPos;
                CCache__SetFrameDelay(8);
                EnemyAI_Mount(mount, mount->ai, p);
                return CPlayer__SetState(p, body, 0x21);
            }
        }
    }
    return 0;
}
