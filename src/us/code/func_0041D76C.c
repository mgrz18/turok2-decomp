/* WRONG CUT: real function 0x0041D76C, size 0xE0 (through 0x0041D84C): splat cut it in two
 * (func_0041D76C, func_0041D7B0) in the middle of an index computation. */
#include "common.h"

typedef struct {
    /* 0x00 */ s32 cursor;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 length;
    /* 0x0C */ u8 name[12];
} NameEntry; /* 0x18 bytes */

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ void *options;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
} MenuCaller;

extern NameEntry D_800C1FB0[];
extern u8 D_80119870[]; /* player list; setup slot names at +0x131B (D_8011AB8B) */
extern s32 func_0025340C(void *players, void *player);
extern void func_0041BC50(void *options);
extern void func_00416454(s32 menu, MenuCaller *c, s32 pool);

/* Name entry: confirm. Copies the name (blanks for unused characters) into the setup slot;
 * an empty name resets the player's options. */
s32 func_0041D76C(s32 menu, MenuCaller *c, s32 pool) {
    Player *player = c->player;
    s32 i = func_0025340C(D_80119870, player);
    u8 *src = D_800C1FB0[i].name;
    u8 *dst = D_80119870 + 0x131B + (i << 6);
    s32 n = 0;
    s32 j;
    u8 ch;

    for (j = 0; j < 8; j++) {
        ch = *src++;
        if (ch == 0) {
            ch = ' ';
        } else {
            n++;
        }
        *dst++ = ch;
    }
    *dst = 0;
    if (n == 0) {
        func_0041BC50(player->options);
    }
    func_00416454(menu, c, pool);
    return 1;
}
