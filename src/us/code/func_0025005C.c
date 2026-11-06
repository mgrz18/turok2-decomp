#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/region.h"
#include "globals/camera.h"

extern u8 gScene[];
extern char D_800A6EA8[]; /* "Fly Mode Off" */
extern f32 gFrameIncrement;    /* frame time */

void Math_EaseToward(f32 *value, f32 target, f32 rate);
f32 Math_StepClamped(f32 value, f32 target, f32 rate);
f32 Math_DecayToZero(f32 value, f32 rate);
f32 func_002982F0(f32 x); /* sin */
f32 func_002974C0(f32 x); /* cos */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
CGameRegion *CScene__FindRegionAt(void *world, CVector3 *pos);
s32 CPlayer__SetState(CPlayer *player, CGameObjectInstance *body, s32 mode);
void CCameraPool__AddText(u8 *game, void *hud, char *msg);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* Fly mode (cheat): turn and pitch with the stick, fly forward/sideways and
 * up/down with the buttons; holding all three 0xE000 buttons and pressing one
 * again leaves it, dropping onto the middle of the floor under the player. */
void CPlayer__UpdateFlyCheat(CPlayer *player, CGameObjectInstance *body) {
    CVector3 move;
    CGameRegion *floor;
    u32 held;

    if (player->m_Mode == 0 && !(player->m_ContState.m_dwButtons & 0xE000)) {
        player->m_Mode = 1;
    }
    player->m_vHeadRotOffset.z = 0.0f;
    Math_EaseToward(&body->m_RotY, body->m_RotY + player->m_ContState.m_pContState->stickX * 0.0043633236f, 0.9f);
    Math_EaseToward(&player->m_vHeadRotOffset.x, player->m_vHeadRotOffset.x + -player->m_ContState.m_pContState->stickY * 0.0021816618f, 0.9f);
    if (player->m_ContState.m_FinalMoveY != 0.0f) {
        player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * 8.533333f,
                                        ABS(player->m_ContState.m_FinalMoveY) * 102.399994f);
    } else {
        player->m_Speed = Math_DecayToZero(player->m_Speed, 17.066666f);
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * 8.533333f,
                                       ABS(player->m_ContState.m_FinalMoveX) * 102.399994f);
    } else {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, 17.066666f);
    }
    held = player->m_ContState.m_dwButtons;
    if (held & 0x10) {
        body->m_vVelocity.y = Math_StepClamped(body->m_vVelocity.y, 5.9733334f, 71.68f);
    } else if (held & 0x2020) {
        body->m_vVelocity.y = Math_StepClamped(body->m_vVelocity.y, -5.9733334f, 71.68f);
    } else {
        body->m_vVelocity.y = Math_DecayToZero(body->m_vVelocity.y, 11.946667f);
    }
    move.x = player->m_Speed * func_002982F0(body->m_RotY + 3.1415927f);
    move.y = body->m_vVelocity.y;
    move.z = player->m_Speed * func_002974C0(body->m_RotY + 3.1415927f);
    move.x += player->m_SideSpeed * func_002982F0(body->m_RotY - 1.5707964f);
    move.z += player->m_SideSpeed * func_002974C0(body->m_RotY - 1.5707964f);
    CVector3__Scale(&move, &move, gFrameIncrement);
    CVector3__Add(&player->m_vDesiredPos, &player->m_vDesiredPos, &move);
    floor = CScene__FindRegionAt(gScene, &body->header.m_vPos);
    if (floor != NULL) {
        body->header.m_pCurrentRegion = floor;
    }
    if (player->m_Mode == 1 && (player->m_ContState.m_dwButtons & 0xE000) == 0xE000 && (player->m_ContState.m_dwButtonsSingle & 0xE000)) {
        CPlayer__SetState(player, body, 2);
        CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, D_800A6EA8);
        if (floor == NULL) {
            floor = body->header.m_pCurrentRegion;
            body->header.m_vPos = *floor->m_pVertices[0];
            CVector3__Add(&body->header.m_vPos, &body->header.m_vPos, floor->m_pVertices[1]);
            CVector3__Add(&body->header.m_vPos, &body->header.m_vPos, floor->m_pVertices[2]);
            CVector3__Scale(&body->header.m_vPos, &body->header.m_vPos, 0.33333334f);
        }
    }
}
