#include "common.h"

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 unk3E;
} PlayerOptions;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[0xB2C - 0x51C];
    /* 0xB2C */ s32 markersOn;
    /* 0xB30 */ u8 padB30[0xB3C - 0xB30];
    /* 0xB3C */ s32 sprite2;
    /* 0xB40 */ f32 pos2[3];
    /* 0xB4C */ u8 padB4C[0xB5C - 0xB4C];
    /* 0xB5C */ s32 sprite1;
    /* 0xB60 */ f32 frame;
    /* 0xB64 */ f32 pos1[3];
} Player;

extern f32 D_800B6D28;      /* frame step */
extern u8 D_8011AAF1;
extern u8 D_8011AAFC;
extern u8 D_8011AB01;
extern u8 D_8011AB06;
extern s32 D_8011ACD4;
extern s32 D_8011ACE4;

void func_00279028(s32);
void func_0024A678(void *view, f32 *pos, s32 sprite, f32 frame, f32 scale, f32 size);

/* Draws the player's two target markers. */
void func_0024A86C(Player *player, void *view) {
    s32 sprite;
    u8 show;
    f32 scale;

    player->frame += D_800B6D28;
    if (player->markersOn == 0 || player->options->unk3E == 1) {
        return;
    }
    func_00279028(0xFF);

    sprite = player->sprite1;
    if (sprite == 0x2DA && D_8011AAF1 != 0) {
        if (D_8011ACE4 == 0) {
            if (D_8011ACD4 != 0) {
                show = D_8011AB06;
            } else {
                show = D_8011AAFC;
            }
        } else {
            show = D_8011AB01;
        }
    } else {
        show = 1;
    }
    if (sprite != -1) {
        switch (sprite) {
        case 0x2DA:
            scale = 8.0f;
            break;
        case 0x2BC:
        case 0x2E4:
        case 0x320:
        case 0x32A:
        case 0x32B:
            scale = 32.0f;
            break;
        default:
            scale = 16.0f;
            break;
        }
        if (show) {
            func_0024A678(view, player->pos1, sprite, player->frame, scale, 1.0f);
        }
    }

    sprite = player->sprite2;
    if (sprite == 0x2DA && D_8011AAF1 != 0) {
        if (D_8011ACE4 == 0) {
            if (D_8011ACD4 != 0) {
                show = D_8011AB06;
            } else {
                show = D_8011AAFC;
            }
        } else {
            show = D_8011AB01;
        }
    } else {
        show = 1;
    }
    if (sprite != -1) {
        switch (sprite) {
        case 0x2DA:
            scale = 8.0f;
            break;
        case 0x2BC:
        case 0x2C6:
        case 0x2C7:
        case 0x2E4:
        case 0x320:
            scale = 32.0f;
            break;
        default:
            scale = 16.0f;
            break;
        }
        if (show) {
            func_0024A678(view, player->pos2, sprite, player->frame, scale, 1.0f);
        }
    }
}
