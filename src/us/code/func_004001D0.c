#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 pad08[0x19 - 0x8];
    /* 0x19 */ u8 multiplayer;
} GameSub004001D0;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1268];
    /* 0x1268 */ GameSub004001D0 sub;
} Game004001D0;

typedef struct {
    /* 0x00 */ s32 state;
    /* 0x04 */ u8 pad04[0x1C - 0x4];
    /* 0x1C */ s32 locked;
} Cinema004001D0;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *options;
    /* 0x520 */ u8 pad1[0xA64 - 0x520];
    /* 0xA64 */ s32 held;
    /* 0xA68 */ s32 pressed;
    /* 0xA6C */ s32 released;
    /* 0xA70 */ u8 pad2[0xB8C - 0xA70];
    /* 0xB8C */ s32 unkB8C;
} Player004001D0;

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

extern Game004001D0 D_80119870;
extern Cinema004001D0 D_8011AC88;
extern f32 D_800B6D28;
Player004001D0 *func_002532A8(Game004001D0 *game);
s32 func_00416AF8(u8 *a0);
void func_002117DC(f32 *value, f32 target, f32 rate);

void func_004001D0(Look004001D0 *look) {
    Player004001D0 *player;
    s32 held;
    GameSub004001D0 *sub = &D_80119870.sub;

    if (sub->multiplayer) {
        return;
    }
    player = func_002532A8(&D_80119870);
    if (player == NULL) {
        return;
    }
    if (look->mode == 0) {
        if (!func_00416AF8(player->options + 0x53C) && (player->pressed & 0x20) && player->unkB8C == 0) {
            look->mode = 1;
        }
    } else if (player->unkB8C != 0) {
        look->mode = 0;
    } else if (look->mode == 1) {
        if ((player->released & 0x20) || !(player->held & 0x20)) {
            look->mode = 2;
        }
    } else if (player->released & 0x20) {
        look->mode = 0;
    }
    if (look->mode != 0 && D_8011AC88.state == 13 && D_8011AC88.locked == 0) {
        if (look->state != 1 && look->state != 2) {
            look->state = 1;
        }
    } else if (look->state != 0 && look->state != 3) {
        look->state = 3;
    }
    if (look->state != 0) {
        held = player->held;
        if (held & 0x20) {
            if (held & 0x200) {
                look->yawTarget += D_800B6D28 * 153.59999f;
            } else if (held & 0x100) {
                look->yawTarget -= D_800B6D28 * 153.59999f;
            }
            held = player->held;
            if (held & 0x800) {
                look->pitchTarget += D_800B6D28 * 153.59999f;
            } else if (held & 0x400) {
                look->pitchTarget -= D_800B6D28 * 153.59999f;
            }
        } else {
            look->yawTarget = 0.0f;
            look->unk30 = 0.0f;
            look->pitchTarget = 0.0f;
        }
        func_002117DC(&look->yaw, look->yawTarget, 0.33333334f);
        func_002117DC(&look->pitch, look->pitchTarget, 0.25f);
    }
    switch (look->state) {
        case 2:
            break;
        case 1:
            look->blend += D_800B6D28 * 0.13333334f;
            if (look->blend >= 1.0f) {
                look->blend = 1.0f;
                look->state = 2;
            }
            break;
        case 3:
            look->blend -= D_800B6D28 * 0.26666668f;
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
