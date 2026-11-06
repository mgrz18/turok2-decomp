/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_16.CPP, func_8022470C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

extern f32 D_800B6D28;      /* frame time */
extern char D_800ADE70;     /* standing look tuning */
extern char D_800ADE88;     /* crouching look tuning */
extern char D_800ADDBC;     /* standing move tuning */
extern char D_800ADE4C;     /* crouching move tuning */

void Math_EaseToward(f32 *value, f32 target, f32 rate);
void CPlayer__BobViewStride(CPlayer *player, CGameObjectInstance *body);
void CPlayer__TurnAndLook(CPlayer *player, CGameObjectInstance *body, void *tuning);
void CPlayer__MoveOnFoot(CPlayer *player, CGameObjectInstance *body, void *tuning);
s32 CPlayer__SetState(CPlayer *player, CGameObjectInstance *body, s32 mode);
void Sound__Play(s16 sound, CVector3 pos, s32, s32);

static __inline__ s32 try_jump(CPlayer *player, CGameObjectInstance *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->m_AI.m_dwFlags & 0x6000)
        && (body->header.m_pIntelligence->m_dwTypeFlags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        CPlayer__SetState(player, body, 4);
        return 1;
    }
    return 0;
}

/* Walking: bob the view, turn and move with the crouching or standing
 * tunings, hand off to a jump or to standing, play the wading sound in water,
 * then pick the walking animation from the direction. */
void PlayerWalk_Update(CPlayer *player, CGameObjectInstance *body) {
    f32 forward;
    f32 strafe;
    f32 time;
    s32 crouching;

    CPlayer__BobViewStride(player, body);
    Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    crouching = 5.12f < player->m_DuckOffset;
    if (crouching) {
        CPlayer__TurnAndLook(player, body, &D_800ADE88);
    } else {
        CPlayer__TurnAndLook(player, body, &D_800ADE70);
    }
    crouching = 5.12f < player->m_DuckOffset;
    if (crouching) {
        CPlayer__MoveOnFoot(player, body, &D_800ADE4C);
    } else {
        CPlayer__MoveOnFoot(player, body, &D_800ADDBC);
    }
    if (try_jump(player, body)) {
        return;
    }
    if (player->m_Speed == 0.0f && player->m_SideSpeed == 0.0f) {
        CPlayer__SetState(player, body, 2);
    }
    if ((body->m_AI.m_dwFlags & 0x400) && player->m_bOnGround != 0) {
        time = player->m_SfxTimer + D_800B6D28;
        player->m_SfxTimer = time;
        if (player->m_TotalSpeed > 5.12f && time >= 6.0f) {
            player->m_SfxTimer = time - 6.0f;
            Sound__Play(0x3AB, body->header.m_vPos, 0, -1);
        }
    }
    if (body->m_AI.m_dwFlags & 0x6000) {
        if (player->m_Speed >= 0.0f) {
            player->m_CharAnim = 0xA32;
        } else {
            player->m_CharAnim = 0xA37;
        }
        return;
    }
    forward = player->m_Speed;
    if (forward < 0.0f) {
        forward = -forward;
    }
    strafe = player->m_SideSpeed;
    if (forward < (strafe < 0.0f ? -strafe : strafe)) {
        if (player->m_SideSpeed < 0.0f) {
            player->m_CharAnim = 0x802;
        } else {
            player->m_CharAnim = 0x80C;
        }
    } else {
        if (player->m_Speed >= 0.0f) {
            player->m_CharAnim = 0x7DA;
        } else {
            player->m_CharAnim = 0x7DF;
        }
    }
}
