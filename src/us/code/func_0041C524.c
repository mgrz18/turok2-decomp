#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

typedef struct {
    /* 0x00 */ u8 pad0[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
} MenuCaller;

extern PlayerOptions D_8011AB18; /* D_8011AAD8 + 0x40: default options */
extern char D_800B4050[];
extern char D_800B4068[];
extern char D_800C1318[]; /* "  %2d"-style */
extern char D_800C1320[];
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Label of the player's 0x80-centred setting at options+0x29. */
s32 func_0041C524(MenuItemTmpl *a, MenuCaller *c) {
    PlayerOptions *options = &D_8011AB18;
    s32 v;

    if (c->player != NULL && c->player->options != NULL) {
        options = c->player->options;
    }
    v = (options->unk29 - 0x80) / 8;
    if (v == 0) {
        a->text = D_800B4050;
    } else if (v > 0) {
        a->text = D_800B4068;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1318, v);
    } else {
        a->text = D_800B4068;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1320, v);
    }
    return 0;
}
