#include "common.h"
#include "structs/menu.h"

/* The multiplayer settings, D_8011AAD8 (0x184 bytes, see func_0042F514). */
typedef struct MenuSettings {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
    /* 0x22 */ s8 unk22;
    /* 0x23 */ s8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s8 unk26;
    /* 0x27 */ s8 unk27;
    /* 0x28 */ s8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ s8 unk2A;
    /* 0x2B */ s8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ u8 unk2E;
} MenuSettings;

extern MenuSettings D_8011AAD8;

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u8 handicap;
} PlayerOptions;

typedef struct MenuPlayer {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
} MenuPlayer;

extern char D_800B41B0[];
extern char D_800B41C0[];
extern char D_800B41D0[];
extern char D_800B41E0[];
extern char D_800B41F0[];
extern char D_800B4200[];

/* Shows the player's handicap: two special values, otherwise a per-port line with the number. */
s32 func_00450F4C(MenuItem *item, Menu *ctx) {
    s32 handicap = ((MenuPlayer *) ctx->player)->options->handicap;

    if (handicap == 0) {
        item->text = D_800B41F0;
    } else if (handicap == 10) {
        item->text = D_800B4200;
    } else {
        switch (ctx->controller->port) {
        case 0:
        default:
            item->text = D_800B41B0;
            break;
        case 1:
            item->text = D_800B41C0;
            break;
        case 2:
            item->text = D_800B41D0;
            break;
        case 3:
            item->text = D_800B41E0;
            break;
        }
        func_0029E3E0(&item->text[func_004160F0(item) - 2], "%02d", handicap);
    }
    return 0;
}
