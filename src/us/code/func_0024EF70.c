#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x29];
    /* 0x29 */ u8 turnSpeed;        /* 0-255, 128 normal */
    /* 0x2A */ u8 lookSpeed;
    /* 0x2B */ s8 lookMode;
    /* 0x2C */ u8 pad2C;
    /* 0x2D */ u8 lookSpring;
} LookOptions;

typedef struct {
    /* 0x00 */ f32 turn;
    /* 0x04 */ f32 turnRate;
    /* 0x08 */ f32 lookRate;
    /* 0x0C */ f32 lookRateScale;
    /* 0x10 */ f32 lookDown;
    /* 0x14 */ f32 lookUp;
} LookTuning;

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ u32 flags;
} Floor;

Floor *GameObject_GetRegionInfo(CGameObjectInstance *body);
void Math_EaseToward(f32 *value, f32 target, f32 rate);

/* Turns the body and tilts the view from the stick, at the rates of the
 * tuning scaled by the player's speed settings and slowed while zoomed. */
void CPlayer__TurnAndLook(CPlayer *player, CGameObjectInstance *body, LookTuning *tuning) {
    LookOptions *options;
    Floor *floor;
    f32 zoomScale;
    f32 turnRate;
    f32 lookRate;
    f32 tilt;
    f32 speed;

    if (player->m_bHeadMovementDisabled != 0) {
        return;
    }
    options = player->m_pOptions;
    zoomScale = 1.0f - player->m_ZoomBlend * 0.99f;
    speed = options->turnSpeed * (1.0f / 128) - 1.0f;
    turnRate = speed * 0.5f + 0.875f;
    turnRate *= zoomScale * tuning->turnRate;
    speed = options->lookSpeed * (1.0f / 128) - 1.0f;
    lookRate = speed * 0.4f + 0.6f;
    lookRate *= zoomScale * tuning->lookRate * tuning->lookRateScale;
    switch (options->lookMode) {
        case 0:
            if (!options->lookSpring) {
                lookRate = 0.8f;
            }
            break;
        case 1:
            lookRate = 0.5f;
            break;
    }
    floor = GameObject_GetRegionInfo(body);
    if (floor == NULL || (floor->flags & 0x6000) != 0x2000) {
        if (tuning->turn != 0.0f) {
            Math_EaseToward(&body->m_RotY, body->m_RotY + player->m_ContState.m_FinalLookX * 80.0f * tuning->turn * 0.017453294f, turnRate);
        }
    }
    tilt = -player->m_ContState.m_FinalLookY * 80.0f;
    if (tilt < 0.0f) {
        tilt = tilt * -0.71619713f * tuning->lookDown;
    } else {
        tilt = tilt * 0.71619713f * tuning->lookUp;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.x, tilt * 0.017453294f, lookRate);
}
