/* Second VM module copy of func_0042E29C (same source, linked 0x0042E1BC). */
#include "common.h"
#include "structs/game.h"

typedef struct Options {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} Options;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0xE2];
    /* 0x00E2 */ s8 unkE2;
    /* 0x00E3 */ u8 padE3[0x144 - 0xE3];
    /* 0x0144 */ s32 health;
    /* 0x0148 */ u8 pad148[0x518 - 0x148];
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 pad51C[0x524 - 0x51C];
    /* 0x0524 */ s32 health2;       /* 8.8 */
    /* 0x0528 */ s16 lives;
    /* 0x052A */ s16 keys;
    /* 0x052C */ u8 pad52C[0x92C - 0x52C];
    /* 0x092C */ s32 warp;
    /* 0x0930 */ u8 pad930[0xA28 - 0x930];
    /* 0x0A28 */ f32 unkA28;
    /* 0x0A2C */ u8 padA2C[0xA50 - 0xA2C];
    /* 0x0A50 */ s32 controller;
    /* 0x0A54 */ u8 padA54[0x1298 - 0xA54];
    /* 0x1298 */ s32 unk1298;
    /* 0x129C */ u8 pad129C[0x12A4 - 0x129C];
    /* 0x12A4 */ s32 unk12A4;
    /* 0x12A8 */ s32 unk12A8;
    /* 0x12AC */ s32 unk12AC;
    /* 0x12B0 */ s32 unk12B0;
} Player;

typedef struct GameState {
    s32 state;
} GameState;

extern s32 D_80119878; /* player count */
extern u8 D_8011AAF1;  /* multiplayer */
extern GameState D_8011AC88;
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A6AC8; /* 60.0f, in the engine's .rodata */

extern void func_0024E5F0(Player *p, Player *q, s32);
extern void func_00285BD0(s32 controller, s32 state);
extern void func_002540E4(Player *p);
extern void func_00248340(Player *p);
extern void func_0027580C(s32 sound);
extern void func_00248A8C(Player *p);
extern void func_00248BA8(Player *p, s32 warp, s32 arg2, s32 respawn);

void func_004671BC(Player *p) {
    s32 mode = (D_80119878 >= 2) * 2;
    GameSettings *flags = &D_8011AAD8;

    if (!(flags->cheatFlags & 2) || flags->multiplayer != 0) {
        p->keys--;
    }
    if (p->keys <= 0) {
        if (D_8011AAF1 == 0 || p->unkE2 != 0 || p->options->tagMode != 0) {
            func_0024E5F0(p, p, 15);
        }
        return;
    }
    func_00248BA8(p, p->warp, mode, 1);
    if (D_8011AAF1 != 0 || (func_00285BD0(p->controller, 1), D_8011AAF1 != 0)) {
        func_002540E4(p);
    } else {
        p->health2 = 0x6400;
        p->health = 0x6400;
    }
    p->unkA28 = D_800A6AC8;
    func_00248340(p);
    p->unk1298 = 0;
    p->unk12A8 = 0;
    p->unk12AC = 0;
    p->unk12B0 = 0;
    p->unk12A4 = 0;
    if (D_8011AC88.state == 13 && D_8011AAF1 == 0 && D_800B6D20 >= 11) {
        if (p->keys < 11) {
            func_0027580C(D_800AE2B4[p->keys]);
        }
    }
    if (D_8011AAF1 != 0) {
        func_00248A8C(p);
    }
}
