#include "common.h"
#include "structs/game.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "globals/player.h"
#include "protos/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 pos;
} Ui;

void CPlayer__TurnAndLook(CPlayer *player, CGameObjectInstance *body, void *tuning);
void CPlayer__MoveInWater(CPlayer *player, CGameObjectInstance *body, void *tuning);
f32 func_002982F0(f32 x);
f32 Random_Float(f32 scale);
void Sound__Play(s16 sound, CVector3 pos, void *source, s32);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_State = 0;
    s->m_Turn = 0.0f;
    s->m_nCursor = -1;
    s->m_Open = 0.0f;
}

/* Swimming: sway, move with the swimming tuning, and on surfacing (after two
 * time units) gasp for air and leave the state for falling, wading or
 * walking; then pick the animation from the climb and play strokes. */
void PlayerSwim_Update(CPlayer *player, CGameObjectInstance *body) {
    s32 idle;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 0xA;
    } else {
        player->m_Mode = 0xB;
    }
    CPlayer__TurnAndLook(player, body, &gSwimLookTuning);
    player->m_vHeadWobbleRotOffset.x = func_002982F0(player->m_ModeTime * 0.06981318f) * 2 * 0.017453294f;
    player->m_vHeadWobbleRotOffset.y = func_002982F0(player->m_ModeTime * 0.087266475f) * 2 * 0.017453294f;
    player->m_vHeadWobbleRotOffset.z = func_002982F0(player->m_ModeTime * 0.104719765f) * 2 * 0.017453294f;
    CPlayer__MoveInWater(player, body, &gSwimMoveTuning);
    if (!(body->m_AI.m_dwFlags & 0x80) && 2.0f < player->m_ModeTime) {
        reset_strokes(&player->m_Barrel);
        body->m_vVelocity.y = 0.0f;
        if (!gEngineApp_m_Settings.multiplayer) {
            if (player->m_bOxygenOut) {
                Sound__Play(0x3AC3, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
            } else if (52.5f < player->m_Oxygen) {
                Sound__Play(0x3AC2, ((Ui *)player->m_pCamera)->pos, &((Ui *)player->m_pCamera)->pos, -1);
            }
        }
        if (body->m_AI.m_dwFlags & 0xC0000) {
            CPlayer__SetState(player, body, 2);
        } else if (body->m_AI.m_dwFlags & 0x200) {
            CPlayer__SetState(player, body, 9);
        } else {
            CPlayer__SetState(player, body, 3);
        }
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
    if (5.12f < player->m_TotalSpeed && 45.0f < player->m_SfxTimer) {
        player->m_SfxTimer = Random_Float(15.0f);
        Sound__Play(gSwimMoveTuning.impulseSound, body->header.m_vPos, NULL, -1);
    }
}
