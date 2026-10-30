/* WRONG CUT: real function 0x00418294, size 0xEC (through 0x00418380): splat cut it in two
 * (func_00418294, func_004182E8) in the middle of a call's argument setup. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s8 team;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[4];
    /* 0x520 */ s32 character;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
    /* 0x20 */ void *controller;
} MenuCaller;

typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 sound;
} CharacterSound;

extern CharacterSound D_800C1E78[];
extern u8 D_00438748[];  /* menu template */

extern s32 func_002855E8(void *controller);
extern s32 func_00416558(MenuCaller *c, s32 value, s32 step, s32 min, s32 max, s32 wrap);
extern void func_0042E430(Player *player, s32 team);
extern void func_00416214(s32 menu, s32 id, s32 arg);
extern void func_00275624(s32 sound);
extern void func_00416894(s32 pool, MenuCaller *c, void *tmpl);

/* Team selection item: left/right cycles the team, A confirms. */
s32 func_00418294(s32 menu, MenuCaller *c, s32 pool) {
    Player *player = c->player;
    PlayerOptions *options = player->options;

    if (func_002855E8(c->controller)) {
        func_0042E430(player, options->team);
        func_00275624(D_800C1E78[player->character].sound);
        func_00416894(pool, c, D_00438748);
        return 1;
    } else {
        s8 team = func_00416558(c, options->team, 1, 0, 10, 1);

        options->team = team;
        func_0042E430(player, team);
        func_00416214(menu, 0xB, options->team);
        return 0;
    }
}
