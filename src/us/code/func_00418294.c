/* WRONG CUT: real function 0x00418294, size 0xEC (through 0x00418380): splat cut it in two
 * (func_00418294, func_004182E8) in the middle of a call's argument setup. */
#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
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
extern void func_0042E430(CPlayer *player, s32 team);
extern void func_00416214(s32 menu, s32 id, s32 arg);
extern void func_00275624(s32 sound);
extern void func_00416894(s32 pool, MenuCaller *c, void *tmpl);

/* Team selection item: left/right cycles the team, A confirms. */
s32 func_00418294(s32 menu, MenuCaller *c, s32 pool) {
    CPlayer *player = c->player;
    CPlayerOptions *options = player->m_pOptions;

    if (func_002855E8(c->controller)) {
        func_0042E430(player, options->m_nCharacter);
        func_00275624(D_800C1E78[player->m_Data.m_nDeathmatchCharacter].sound);
        func_00416894(pool, c, D_00438748);
        return 1;
    } else {
        s8 team = func_00416558(c, options->m_nCharacter, 1, 0, 10, 1);

        options->m_nCharacter = team;
        func_0042E430(player, team);
        func_00416214(menu, 0xB, options->m_nCharacter);
        return 0;
    }
}
