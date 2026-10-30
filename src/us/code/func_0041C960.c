#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ void *controller;
} C960Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ C960Player *player;
} C960Arg;

extern s32 func_0025340C(void *players, C960Player *player);
extern void func_00416644(void *pool, void *screen, C960Player *p, void *controller, s32);
extern void func_00421250(void);
extern u8 D_0043B6CC[];   /* menu template */
extern u8 D_80119870[];   /* player list; the menu pool is at +0x5BC (D_80119E2C) */

s32 func_0041C960(s32 arg0, C960Arg *arg1) {
    C960Player *player;

    player = arg1->player;
    func_0025340C(D_80119870, player);
    func_00421250();
    func_00416644(D_80119870 + 0x5BC, D_0043B6CC, player, player->controller, 0);
    return 1;
}
