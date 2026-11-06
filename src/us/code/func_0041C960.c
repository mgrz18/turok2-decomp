#include "common.h"
#include "structs/game.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ void *controller;
} C960Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ C960Player *player;
} C960Arg;

extern s32 CPlayerPool__GetIndex(void *players, C960Player *player);
extern void MenuPool_OpenMenu(void *pool, void *screen, C960Player *p, void *controller, s32);
extern void func_00421250(void);
extern u8 D_0043B6CC[];   /* menu template */

s32 StatsMenu_SaveProfile(s32 arg0, C960Arg *arg1) {
    C960Player *player;

    player = arg1->player;
    CPlayerPool__GetIndex(&D_80119870.m_PlayerList, player);
    func_00421250();
    MenuPool_OpenMenu(D_80119870.m_MenuPool, D_0043B6CC, player, player->controller, 0);
    return 1;
}
