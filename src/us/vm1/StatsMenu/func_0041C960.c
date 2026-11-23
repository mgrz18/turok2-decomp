#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "protos/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ void *controller;
} C960Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ C960Player *player;
} C960Arg;

extern void Pak_ClearDamagedContinued(void);

s32 StatsMenu_SaveProfile(s32 arg0, C960Arg *arg1) {
    C960Player *player;

    player = arg1->player;
    CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    Pak_ClearDamagedContinued();
    MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakSaveProfileMenuDesc, player, player->controller, 0);
    return 1;
}
