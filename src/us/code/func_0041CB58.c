#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player_options.h"
#include "globals/game.h"
#include "protos/player.h"


extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the player's kills count into the label. */
s32 StatsMenu_FragsText(MenuItem *a, Menu *c) {
    s32 n = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player)].m_Kills;

    sprintf(a->text + (MenuItem_TextLength(a) - 4), sStatsMenuFmt_04d, n);
    return 0;
}
