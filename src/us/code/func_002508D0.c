/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_17.CPP, func_80224C4C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/intelligence.h"
#include "globals/engine.h"
#include "globals/player.h"
#include "protos/player.h"

void Math_EaseToward(f32 *value, f32 target, f32 rate);
void CPlayer__TurnAndLook(CPlayer *player, CGameObjectInstance *body, void *tuning);
void CPlayer__MoveOnFoot(CPlayer *player, CGameObjectInstance *body, void *tuning);
f32 GameObject_GetWaterHeight(CGameObjectInstance *body);
f32 func_002982F0(f32 x);
void Sound__Play(s16 sound, CVector3 pos, sptr, s32);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_State = 0;
    s->m_Turn = 0.0f;
    s->m_nCursor = -1;
    s->m_Open = 0.0f;
}

static __inline__ s32 try_jump(CPlayer *player, CGameObjectInstance *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->m_AI.m_dwFlags & 0x6000)
        && (body->header.m_pIntelligence->m_dwTypeFlags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        CPlayer__SetState(player, body, 4);
        return 1;
    }
    return 0;
}

/* Wading: leave the state once out of the water, dive when looking down hard
 * while lifting, else follow the water surface, sway, and pick the animation
 * from the climb; moving plays the tuning's stroke sound now and then. */
void PlayerWade_Update(CPlayer *player, CGameObjectInstance *body) {
    s32 idle;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 8;
    } else {
        player->m_Mode = 9;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    CPlayer__MoveOnFoot(player, body, &gWadeMoveTuning);
    if (!(body->m_AI.m_dwFlags & 0x200)) {
        reset_strokes(&player->m_Barrel);
        CPlayer__SetState(player, body, 3);
        return;
    }
    if (player->m_vHeadRotOffset.x * 57.295776f < -40.0f && player->m_ContState.m_FinalMoveY > 0.0f) {
        player->m_vDesiredPos.y = GameObject_GetWaterHeight(body) - player->m_vEyeOffset.y - 51.199997f;
        CPlayer__SetState(player, body, 0xB);
        return;
    }
    if (!(body->m_AI.m_dwFlags & 0xC0000)) {
        body->m_vVelocity.y = 0.0f;
        Math_EaseToward(&player->m_vDesiredPos.y, GameObject_GetWaterHeight(body) - player->m_vEyeOffset.y * 0.6f, 0.5f);
    }
    player->m_vHeadWobbleRotOffset.x = func_002982F0(player->m_ModeTime * 0.06981318f) * 0.043633237f;
    player->m_vHeadWobbleRotOffset.y = func_002982F0(player->m_ModeTime * 0.087266475f) * 0.043633237f;
    player->m_vHeadWobbleRotOffset.z = func_002982F0(player->m_ModeTime * 0.104719765f) * 0.043633237f;
    if (try_jump(player, body)) {
        return;
    }
    idle = 0;
    if (player->m_CharAnim == 0x1144) {
        idle = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!idle) {
        if (1.024f <= player->m_Speed) {
            player->m_CharAnim = 0x8A2;
        } else if (player->m_Speed <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    player->m_SfxTimer += gFrameIncrement;
    if (5.12f < player->m_TotalSpeed && 11.25f < player->m_SfxTimer) {
        player->m_SfxTimer = 0.0f;
        Sound__Play(gWadeMoveTuning.impulseSound, body->header.m_vPos, 0, -1);
    }
}
