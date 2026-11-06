/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225F20.c, func_00225F20, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

/* The ridden creature (an AI actor). */
typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x34];
    /* 0x174 */ s8 aiState;
} Mount;

extern f32 D_800B6D28;          /* frame time */
extern f32 D_800ADDE4;          /* charge speed */
extern f32 D_800AE344;          /* step length */
extern CVector3 D_800ADCB0;        /* rider view offset */
extern CVector3 D_800ADCBC;        /* rider seat */
extern char D_800ADEA0;         /* riding look tuning */
extern char D_800ADDE0;         /* riding forward move tuning */
extern char D_800ADE04;         /* riding backward move tuning */

f32 func_002974C0(f32 x);
void Math_EaseToward(f32 *value, f32 target, f32 rate);
void CPlayer__TurnAndLook(CPlayer *player, CGameObjectInstance *body, void *tuning);
void CPlayer__MoveOnFoot(CPlayer *player, CGameObjectInstance *body, void *tuning);
void CAI__SetMode(Mount *actor, void *ai, s32 state);
void CCamera__SetShake(void *ui, f32 a, f32 b, f32 c, f32 d, s32 e, CVector3 pos);
void CMtxF__SetRotateY(CMtxF *m, f32 angle);
void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);

/* Riding: the mount sways the view as it turns, charges on button 8, shakes
 * the screen at each step and sets the rider's view offset. */
void CPlayer__UpdateRiding(CPlayer *player, CGameObjectInstance *body) {
    CMtxF m;
    Mount *mount;
    CVector3 *offset;
    f32 turn;
    f32 yaw;
    f32 fwd;
    f32 t;
    f32 step;
    f32 dist;
    f32 next;
    f32 shake;

    mount = ((Mount *)player->m_pOnEnemy);
    turn = player->m_ContState.m_FinalLookX * (((func_002974C0(player->m_RideEnemyTime * 0.2792527f) + 1.0f) * 0.01f + 0.06f) * 80.0f);
    player->m_RideEnemyTime += D_800B6D28;
    turn *= 0.017453294f;
    yaw = body->m_RotY;
    Math_EaseToward(&body->m_RotY, yaw + turn, 0.9f);
    Math_EaseToward(&player->m_vHeadRotOffset.z, ((yaw - body->m_RotY) + player->m_ContState.m_FinalLookX * 8.0f) * 0.017453294f, 0.1f);
    CPlayer__TurnAndLook(player, body, &D_800ADEA0);

    offset = NULL;
    if (player->m_ContState.m_dwButtonsDouble & 8) {
        CAI__SetMode(mount, mount->ai, 0x2E);
        player->m_FleeTime = 90.0f;
        player->m_Speed = D_800ADDE4 * 2;
    }

    fwd = player->m_Speed;
    dist = player->m_OnEnemyStompTime;
    if (fwd < 0.0f) {
        fwd = -fwd;
    }
    if ((player->m_SideSpeed * 2 < 0.0f ? -(player->m_SideSpeed * 2) : player->m_SideSpeed * 2) < fwd) {
        step = player->m_Speed;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    } else {
        step = player->m_SideSpeed * 2;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    }
    player->m_OnEnemyStompTime = next;
    if (D_800AE344 < next) {
        player->m_OnEnemyStompTime = next - D_800AE344;
        if (mount->aiState != 0x2E) {
            player->m_Speed *= 0.5f;
            player->m_SideSpeed *= 0.5f;
        }
        t = player->m_Speed;
        if (t < 0.0f) {
            shake = -t * 2.5f;
        } else {
            shake = t * 2.5f;
        }
        if (50.0f < shake) {
            shake = 50.0f;
        }
        CCamera__SetShake(player->m_pCamera, 0.0f, shake, 0.0f, 1.0f, 1, body->header.m_vPos);
    }

    if (player->m_Speed > 0.0f) {
        CPlayer__MoveOnFoot(player, body, &D_800ADDE0);
    } else {
        CPlayer__MoveOnFoot(player, body, &D_800ADE04);
    }
    t = player->m_Speed;
    if ((t < 0.0f ? -t : t) > 10.24f) {
        player->m_FleeTime = 15.0f;
    }

    if (player->m_OnEnemyObjectType == 0x136) {
        offset = &D_800ADCB0;
        player->m_vOnEnemyScale = D_800ADCBC;
    }
    if (offset != NULL) {
        CMtxF__SetRotateY(&m, body->m_RotY);
        CMtxF__TransformPoint(&m, offset, &player->m_vEyeOffset);
        CVector3__Scale(&player->m_vEyeOffset, &player->m_vEyeOffset, 10.24f);
    }
    player->m_RequestedWeapon = 0x13;
}
