#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

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

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
} Body;

typedef struct {
    /* 0x00 */ Mount *current; /* what the player is aiming at */
} CollisionResult;

extern CollisionResult D_800F1CE0;
extern CVector3 D_800ADCBC;             /* rider seat */
extern u8 D_800F5EC0[][0x224];      /* controller settings, one per player */

f32 func_00246310(Body *body, CVector3 point);   /* angle to a point */
void func_0020EF60(CVector3 *out, CVector3 *in, f32 s);
void func_002470E8(void *controls, void *settings);
f32 func_00253CFC(CPlayer *player);
void func_00206068(s32);
void func_0025EB80(Mount *mount, void *ai, CPlayer *player);
s32 func_0024E5F0(CPlayer *player, Body *body, s32 arg2);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

static __inline__ void setVec3(CVector3 *v, f32 x, f32 y, f32 z) {
    v->z = z;
    v->y = y;
    v->x = x;
}

/* Mounts the creature of type 0x136 in front, if within 45 degrees of the
 * heading and the player is not holding 0x8000: the player takes its seat
 * and heading, and the movement state is reset. */
s32 func_0024EAF8(CPlayer *p, Body *body) {
    Mount *mount;

    if (p->m_pOnEnemy == NULL && !(p->m_ModeBeforeFlags & 0x8000)) {
        if (D_800F1CE0.current != NULL && D_800F1CE0.current->kind == 1) {
            mount = D_800F1CE0.current;
            if (mount->type == 0x136 && ABS(func_00246310(body, mount->pos)) < 0.785398245f) {
                p->m_pOnEnemy = mount;
                p->m_OnEnemyObjectType = 0x136;
                p->m_vOnEnemyScale = D_800ADCBC;
                func_0020EF60(&p->m_vOnEnemyScale, &p->m_vOnEnemyScale, 0.03f);
                body->yaw = mount->yaw;
                func_002470E8(&p->m_ContState, D_800F5EC0[p->m_nController]);
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
                p->m_vEyeOffset.y = func_00253CFC(p);
                p->m_vEyeOffset.z = 0;
                setVec3(&p->m_qGround, 0.0f, 0.0f, 0.0f);
                p->m_ActualCameraVertEyeOffset = 0;
                p->m_qGround.w = 1.0f;
                p->m_CameraMovementScaler = 1.0f;
                p->m_vDesiredPos = p->m_CharacterActor.header.m_vPos;
                func_00206068(8);
                func_0025EB80(mount, mount->ai, p);
                return func_0024E5F0(p, body, 0x21);
            }
        }
    }
    return 0;
}
