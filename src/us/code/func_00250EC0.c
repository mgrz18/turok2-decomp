#include "common.h"
#include "structs/game.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "globals/scene.h"

extern s16 D_800AE312[];        /* per character: climbing step sound (stride 4) */

s32 CGameRegion__IsSteep(void *region);
s32 CPlayer__SetState(CPlayer *player, CGameObjectInstance *body, s32 mode);
void *CScene__GetRegionInfo(u8 *list, void *region);
f32 CGameRegion__GetDownhillYaw(void *region);
f32 Angle_Difference(f32 from, f32 to);
void Math_EaseToward(f32 *value, f32 target, f32 rate);
f32 GameObject_GetRegionTopAbove(CGameObjectInstance *body);
f32 GameObject_GetHeightAboveRegionBottom(CGameObjectInstance *body);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
f32 Math_DecayToZero(f32 value, f32 rate);
f32 Math_StepClamped(f32 value, f32 delta, f32 limit);
void CPlayer__BobView(CPlayer *player, CGameObjectInstance *body);
void Sound__Play2D(s32 sound);
void Sound__Play(s16 sound, CVector3 pos, s32, s32);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

static __inline__ s32 try_jump(CPlayer *player, CGameObjectInstance *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->m_AI.m_dwFlags & 0x6000)
        && (body->header.m_pIntelligence->m_dwTypeFlags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        CPlayer__SetState(player, body, 4);
        return 1;
    }
    return 0;
}

/* Climbing: face the wall, look up or down with the stick (tilting the view
 * back near the top and the bottom of the climb), jump off, climb with the
 * stick and play a step sound every 11.25 time units. */
void PlayerClimb_Update(CPlayer *player, CGameObjectInstance *body) {
    f32 angle;
    f32 side;
    f32 sideRaw;
    f32 scale;
    f32 toTop;
    f32 toBottom;
    f32 lastForward;
    f32 s;
    f32 bob;
    f32 time;
    f32 c;
    s32 nearTop;
    s32 nearBottom;
    s32 up;
    s32 down;
    s32 anim;

    if (!CGameRegion__IsSteep(body->header.m_pCurrentRegion) || !(body->m_AI.m_dwFlags & 0x1000)) {
        player->m_vHeadRotOffset.x = 0.0f;
        CPlayer__SetState(player, body, 2);
        return;
    }
    CScene__GetRegionInfo(&gScene, body->header.m_pCurrentRegion);
    Math_EaseToward(&body->m_RotY, body->m_RotY - Angle_Difference(body->m_RotY, CGameRegion__GetDownhillYaw(body->header.m_pCurrentRegion)), 0.9f);
    Math_EaseToward(&player->m_vHeadRotOffset.y, player->m_ContState.m_FinalLookX * 3.4906588f, 0.25f);
    angle = -player->m_ContState.m_FinalLookY * 80.0f;
    if (angle > 0.0f) {
        angle *= 0.1875f;
    } else {
        angle *= 2.25f;
    }
    /* |turn * 80| * 1.625, written as one product of two picked factors */
    side = player->m_ContState.m_FinalLookX;
    sideRaw = side * 80.0f;
    if (sideRaw < 0.0f) {
        side = 1.625f;
        scale = -sideRaw;
    } else {
        scale = 130.0f;
    }
    angle -= side * scale;
    angle += 90.0f;
    toTop = GameObject_GetRegionTopAbove(body);
    if (toTop < 0.0f) {
        toTop = 0.0f;
    }
    nearTop = 1;
    if (!(toTop <= 184.31999f)) {
        nearTop = 0;
    }
    toBottom = GameObject_GetHeightAboveRegionBottom(body);
    if (toBottom < 0.0f) {
        toBottom = 0.0f;
    }
    nearBottom = 1;
    if (!(toBottom <= 81.92f)) {
        nearBottom = 0;
    }
    if (nearTop && nearBottom) {
        if (toTop < toBottom) {
            nearBottom = 0;
        } else {
            nearTop = 0;
        }
    }
    if (nearTop) {
        angle -= (0.5f - func_002974C0((1.0f - toTop * 0.0054253475f) * 3.1415927f) * 0.5f) * 90.0f;
    }
    if (nearBottom) {
        angle -= (0.5f - func_002974C0((1.0f - toBottom * 0.012207031f) * 3.1415927f) * 0.5f) * 90.0f;
    }
    if (angle < -90.0f) {
        angle = -90.0f;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.x, angle * 0.017453294f, 0.5f);
    if (try_jump(player, body)) {
        s = func_002982F0(body->m_RotY + 6.2831855f);
        c = func_002974C0(body->m_RotY + 6.2831855f);
        body->m_vVelocity.x += s * 384.0f;
        body->m_vVelocity.z += c * 384.0f;
        body->m_RotY += player->m_vHeadRotOffset.y;
        player->m_vHeadRotOffset.y = 0.0f;
        return;
    }
    bob = player->m_HandSide * player->m_Speed * func_002982F0(player->m_ModeTime * 0.13962635f);
    if (player->m_ContState.m_FinalMoveY > 0.0f || player->m_ContState.m_FinalMoveY < 0.0f) {
        Math_EaseToward(&player->m_vHeadRotOffset.z, bob, 0.25f);
    } else {
        Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.9f);
    }
    up = 1;
    if (!(player->m_ContState.m_FinalMoveY > 0.0f)) {
        up = 0;
    }
    down = 0;
    if (!gEngineApp_m_Settings.multiplayer && player->m_ContState.m_FinalMoveY < 0.0f) {
        down = 1;
    }
    lastForward = player->m_Speed;
    if (up || down) {
        player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * 2.048f,
                                        ABS(player->m_ContState.m_FinalMoveY) * 14.3359995f);
    } else {
        player->m_Speed = Math_DecayToZero(lastForward, 4.096f);
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * 2.048f,
                                       ABS(player->m_ContState.m_FinalMoveX) * 14.3359995f);
    } else {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, 4.096f);
    }
    if (player->m_Speed == 0.0f && player->m_SideSpeed == 0.0f) {
        player->m_ModeTime = 0.0f;
        if (lastForward != 0.0f) {
            player->m_HandSide = -player->m_HandSide;
        }
        CPlayer__BobView(player, body);
    } else if ((time = player->m_ModeTime) > 11.25f) {
        player->m_HandSide = -player->m_HandSide;
        player->m_ModeTime = time - 11.25f;
        player->m_Speed *= 0.1f;
        player->m_SideSpeed *= 0.1f;
        if (gEngineApp_m_Settings.multiplayer) {
            if (player->m_pOptions->m_TagMode == 1) {
                Sound__Play2D(0x27DB);
            } else {
                Sound__Play(D_800AE312[player->m_Data.m_nDeathmatchCharacter * 2], body->header.m_vPos, 0, -1);
            }
        } else {
            Sound__Play(0x3AB7, body->header.m_vPos, 0, -1);
        }
    }
    s = func_002982F0(body->m_RotY - 1.5707964f);
    c = func_002974C0(body->m_RotY - 1.5707964f);
    player->m_vDesiredPos.x += s * player->m_SideSpeed * player->m_FrameIncrementScaled;
    player->m_vDesiredPos.z += c * player->m_SideSpeed * player->m_FrameIncrementScaled;
    if (player->m_Speed >= 0.0f) {
        s = func_002982F0(body->m_RotY + 3.1415927f);
        c = func_002974C0(body->m_RotY + 3.1415927f);
        player->m_vDesiredPos.x += s * player->m_Speed * player->m_FrameIncrementScaled;
        player->m_vDesiredPos.z += c * player->m_Speed * player->m_FrameIncrementScaled;
    } else {
        player->m_vDesiredPos.y += player->m_Speed * 0.5f;
    }
    anim = 0xA46;
    if (!(player->m_Speed > 0.0f)) {
        anim = 0xA3C;
    }
    player->m_CharAnim = anim;
}
