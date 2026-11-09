#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "protos/player.h"
#include "structs/camera.h"

/* Free-look camera driven by the C buttons. */
typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 state;
    /* 0x08 */ f32 blend;
    /* 0x0C */ u8 pad0C[0x20 - 0xC];
    /* 0x20 */ f32 yaw;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 pitch;
    /* 0x2C */ f32 yawTarget;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 pitchTarget;
} Look004001D0;

s32 MenuPool_CountOpen(u8 *a0);
void Math_EaseToward(f32 *value, f32 target, f32 rate);

void Automap_Update(Look004001D0 *look) {
    CPlayer *player;
    s32 held;
    GameSettings *sub = &gEngineApp_m_Game.m_Settings;

    if (sub->multiplayer) {
        return;
    }
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (player == NULL) {
        return;
    }
    if (look->mode == 0) {
        if (!MenuPool_CountOpen(((u8 *)player->m_pCamera) + T2_OFFSETOF(CCamera, m_MenuPool)) && (player->m_ContState.m_dwButtonsSingle & 0x20) && player->m_SniperMode == 0) {
            look->mode = 1;
        }
    } else if (player->m_SniperMode != 0) {
        look->mode = 0;
    } else if (look->mode == 1) {
        if ((player->m_ContState.m_dwButtonsSingleNoHold & 0x20) || !(player->m_ContState.m_dwButtons & 0x20)) {
            look->mode = 2;
        }
    } else if (player->m_ContState.m_dwButtonsSingleNoHold & 0x20) {
        look->mode = 0;
    }
    if (look->mode != 0 && gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.paused == 0) {
        if (look->state != 1 && look->state != 2) {
            look->state = 1;
        }
    } else if (look->state != 0 && look->state != 3) {
        look->state = 3;
    }
    if (look->state != 0) {
        held = player->m_ContState.m_dwButtons;
        if (held & 0x20) {
            if (held & 0x200) {
                look->yawTarget += gFrameIncrement * 153.59999f;
            } else if (held & 0x100) {
                look->yawTarget -= gFrameIncrement * 153.59999f;
            }
            held = player->m_ContState.m_dwButtons;
            if (held & 0x800) {
                look->pitchTarget += gFrameIncrement * 153.59999f;
            } else if (held & 0x400) {
                look->pitchTarget -= gFrameIncrement * 153.59999f;
            }
        } else {
            look->yawTarget = 0.0f;
            look->unk30 = 0.0f;
            look->pitchTarget = 0.0f;
        }
        Math_EaseToward(&look->yaw, look->yawTarget, 0.33333334f);
        Math_EaseToward(&look->pitch, look->pitchTarget, 0.25f);
    }
    switch (look->state) {
        case 2:
            break;
        case 1:
            look->blend += gFrameIncrement * 0.13333334f;
            if (look->blend >= 1.0f) {
                look->blend = 1.0f;
                look->state = 2;
            }
            break;
        case 3:
            look->blend -= gFrameIncrement * 0.26666668f;
            if (look->blend <= 0.0f) {
                look->blend = 0.0f;
                look->state = 0;
                look->yaw = 0.0f;
                look->unk24 = 0.0f;
                look->pitch = 0.0f;
                look->yawTarget = 0.0f;
                look->unk30 = 0.0f;
                look->pitchTarget = 0.0f;
            }
            break;
    }
}
