#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player_options.h"


extern CPlayerOptions gEngineApp_m_Settings_slots[]; /* gEngineApp_m_Settings.slots */
extern char D_800C1330[];      /* number format */
extern s32 CPlayerPool__GetIndex(void *players, void *player);
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the player's tags count into the label. */
s32 StatsMenu_TagsText(MenuItem *a, Menu *c) {
    s32 n = gEngineApp_m_Settings_slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player)].m_Tags;

    sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1330, n);
    return 0;
}
