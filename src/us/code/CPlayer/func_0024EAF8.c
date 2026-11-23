#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/collision.h"
#include "globals/collision.h"
#include "globals/player.h"
#include "protos/cache.h"
#include "protos/player.h"

/* The creature to mount (an AI actor). */

f32 AI_AngleToPoint(CGameObjectInstance *body, CVector3 point);   /* angle to a point */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);
void CPlayerContState__Init(void *controls, void *settings);
f32 Actor_GetEyeHeight(CPlayer *player);
void EnemyAI_Mount(CGameObjectInstance *mount, void *ai, CPlayer *player);

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
    CGameObjectInstance *mount;

    if (p->m_pOnEnemy == NULL && !(p->m_ModeBeforeFlags & 0x8000)) {
        if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
            mount = (CGameObjectInstance *)gCollisionResults.m_pInstance;
            if (mount->m_nTypeFlag == 0x136 && ABS(AI_AngleToPoint(body, mount->header.m_vPos)) < 0.785398245f) {
                p->m_pOnEnemy = mount;
                p->m_OnEnemyObjectType = 0x136;
                p->m_vOnEnemyScale = gRidingScale;
                CVector3__Scale(&p->m_vOnEnemyScale, &p->m_vOnEnemyScale, 0.03f);
                body->m_RotY = mount->m_RotY;
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
                EnemyAI_Mount(mount, &mount->m_AI, p);
                return CPlayer__SetState(p, body, 0x21);
            }
        }
    }
    return 0;
}
