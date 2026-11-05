#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/region.h"

extern u8 D_800F7078[];
extern u8 D_801198B0[];
extern char D_800A6EA8[]; /* "Fly Mode Off" */
extern f32 D_800B6D28;    /* frame time */

void func_002117DC(f32 *value, f32 target, f32 rate);
f32 func_0021160C(f32 value, f32 target, f32 rate);
f32 func_0021170C(f32 value, f32 rate);
f32 func_002982F0(f32 x); /* sin */
f32 func_002974C0(f32 x); /* cos */
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
CGameRegion *func_00220A1C(void *world, CVector3 *pos);
s32 func_0024E5F0(CPlayer *player, CGameObjectInstance *body, s32 mode);
void func_0027FC2C(u8 *game, void *hud, char *msg);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* Fly mode (cheat): turn and pitch with the stick, fly forward/sideways and
 * up/down with the buttons; holding all three 0xE000 buttons and pressing one
 * again leaves it, dropping onto the middle of the floor under the player. */
void func_0025005C(CPlayer *player, CGameObjectInstance *body) {
    CVector3 move;
    CGameRegion *floor;
    u32 held;

    if (player->m_Mode == 0 && !(player->m_ContState.m_dwButtons & 0xE000)) {
        player->m_Mode = 1;
    }
    player->m_vHeadRotOffset.z = 0.0f;
    func_002117DC(&body->m_RotY, body->m_RotY + player->m_ContState.m_pContState->stickX * 0.0043633236f, 0.9f);
    func_002117DC(&player->m_vHeadRotOffset.x, player->m_vHeadRotOffset.x + -player->m_ContState.m_pContState->stickY * 0.0021816618f, 0.9f);
    if (player->m_ContState.m_FinalMoveY != 0.0f) {
        player->m_Speed = func_0021160C(player->m_Speed, player->m_ContState.m_FinalMoveY * 8.533333f,
                                        ABS(player->m_ContState.m_FinalMoveY) * 102.399994f);
    } else {
        player->m_Speed = func_0021170C(player->m_Speed, 17.066666f);
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = func_0021160C(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * 8.533333f,
                                       ABS(player->m_ContState.m_FinalMoveX) * 102.399994f);
    } else {
        player->m_SideSpeed = func_0021170C(player->m_SideSpeed, 17.066666f);
    }
    held = player->m_ContState.m_dwButtons;
    if (held & 0x10) {
        body->m_vVelocity.y = func_0021160C(body->m_vVelocity.y, 5.9733334f, 71.68f);
    } else if (held & 0x2020) {
        body->m_vVelocity.y = func_0021160C(body->m_vVelocity.y, -5.9733334f, 71.68f);
    } else {
        body->m_vVelocity.y = func_0021170C(body->m_vVelocity.y, 11.946667f);
    }
    move.x = player->m_Speed * func_002982F0(body->m_RotY + 3.1415927f);
    move.y = body->m_vVelocity.y;
    move.z = player->m_Speed * func_002974C0(body->m_RotY + 3.1415927f);
    move.x += player->m_SideSpeed * func_002982F0(body->m_RotY - 1.5707964f);
    move.z += player->m_SideSpeed * func_002974C0(body->m_RotY - 1.5707964f);
    func_0020EF60(&move, &move, D_800B6D28);
    func_0020EEF8(&player->m_vDesiredPos, &player->m_vDesiredPos, &move);
    floor = func_00220A1C(D_800F7078, &body->header.m_vPos);
    if (floor != NULL) {
        body->header.m_pCurrentRegion = floor;
    }
    if (player->m_Mode == 1 && (player->m_ContState.m_dwButtons & 0xE000) == 0xE000 && (player->m_ContState.m_dwButtonsSingle & 0xE000)) {
        func_0024E5F0(player, body, 2);
        func_0027FC2C(D_801198B0, player->m_pCamera, D_800A6EA8);
        if (floor == NULL) {
            floor = body->header.m_pCurrentRegion;
            body->header.m_vPos = *floor->m_pVertices[0];
            func_0020EEF8(&body->header.m_vPos, &body->header.m_vPos, floor->m_pVertices[1]);
            func_0020EEF8(&body->header.m_vPos, &body->header.m_vPos, floor->m_pVertices[2]);
            func_0020EF60(&body->header.m_vPos, &body->header.m_vPos, 0.33333334f);
        }
    }
}
