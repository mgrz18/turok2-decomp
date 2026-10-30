/* NON-MATCHING: 39 of 129 words differ: the turn rate and look rate computations are scheduled in the other order (look rate first here) */
#include "common.h"

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

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[0x518];
    /* 0x0518 */ LookOptions *options;
    /* 0x051C */ u8 pad51C[0xA54 - 0x51C];
    /* 0x0A54 */ f32 stickX;
    /* 0x0A58 */ f32 stickY;
    /* 0x0A5C */ u8 padA5C[0xAC8 - 0xA5C];
    /* 0x0AC8 */ f32 pitch;
    /* 0x0ACC */ u8 padACC[0xB90 - 0xACC];
    /* 0x0B90 */ f32 zoom;
    /* 0x0B94 */ u8 padB94[0x1284 - 0xB94];
    /* 0x1284 */ s32 locked;
} Player;

Floor *func_00268A2C(Body *body);
void func_002117DC(f32 *value, f32 target, f32 rate);

/* Turns the body and tilts the view from the stick, at the rates of the
 * tuning scaled by the player's speed settings and slowed while zoomed. */
void func_0024EF70(Player *player, Body *body, LookTuning *tuning) {
    LookOptions *options;
    Floor *floor;
    f32 zoomScale;
    f32 turnRate;
    f32 lookRate;
    f32 tilt;
    f32 scale;
    f32 range;

    if (player->locked != 0) {
        return;
    }
    options = player->options;
    zoomScale = 1.0f - player->zoom * 0.99f;
    turnRate = ((options->turnSpeed * (1.0f / 128) - 1.0f) * 0.5f + 0.875f) * (zoomScale * tuning->turnRate);
    lookRate = ((options->lookSpeed * (1.0f / 128) - 1.0f) * 0.4f + 0.6f) * (zoomScale * tuning->lookRate * tuning->lookRateScale);
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
    floor = func_00268A2C(body);
    if (floor == NULL || (floor->flags & 0x6000) != 0x2000) {
        if (tuning->turn != 0.0f) {
            func_002117DC(&body->yaw, body->yaw + player->stickX * 80.0f * tuning->turn * 0.017453294f, turnRate);
        }
    }
    tilt = -player->stickY * 80.0f;
    if (tilt < 0.0f) {
        scale = tilt * -0.71619713f;
        range = tuning->lookDown;
    } else {
        scale = tilt * 0.71619713f;
        range = tuning->lookUp;
    }
    func_002117DC(&player->pitch, scale * range * 0.017453294f, lookRate);
}
