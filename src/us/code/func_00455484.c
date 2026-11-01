#include "common.h"

/* A menu item: its text is a format line whose last characters get the value. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0x14 - 0xC];
    /* 0x14 */ char *text;
} MenuItem;

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
} PlayerOptions;

typedef struct MenuPlayer {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
} MenuPlayer;

typedef struct MenuContext {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ MenuPlayer *player;
} MenuContext;

/* What these read around the multiplayer settings, D_8011AAD8. */
typedef struct MenuSettings {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ u32 unkC;
    /* 0x010 */ u16 unk10;
    /* 0x012 */ u8 pad12[0x19 - 0x12];
    /* 0x019 */ u8 multiplayer;
    /* 0x01A */ u8 pad1A;
    /* 0x01B */ u8 unk1B;
    /* 0x01C */ u8 pad1C[0x40 - 0x1C];
    /* 0x040 */ PlayerOptions defaultOptions;
    /* 0x06B */ u8 pad6B[0x1B0 - 0x6B];
    /* 0x1B0 */ s32 mode;
} MenuSettings;

extern MenuSettings D_8011AAD8;

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B4050[];
extern char D_800B4068[];

/* Shows the player's option unk29 (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 func_00455484(MenuItem *item, MenuContext *menu) {
    PlayerOptions *options = &D_8011AAD8.defaultOptions;
    s32 value;

    if (menu->player != NULL && menu->player->options != NULL) {
        options = menu->player->options;
    }
    value = (options->unk29 - 0x80) / 8;
    if (value == 0) {
        item->text = D_800B4050;
    } else if (value > 0) {
        item->text = D_800B4068;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", value);
    } else {
        item->text = D_800B4068;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], " %03d", value);
    }
    return 0;
}
