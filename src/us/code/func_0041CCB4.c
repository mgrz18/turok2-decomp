#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/player_options.h"

extern s32 gEngineApp_m_Settings_match_tag;         /* match started */
extern CPlayerOptions gEngineApp_m_Settings_slots[]; /* gEngineApp_m_Settings.slots */
extern char D_800C1338[];      /* number format */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* "<name> <count>" line of the score table, for the slot of item index - 2. */
s32 StatsMenu_PlayerRowText(MenuItem *a, Menu *c) {
    s32 slot = a->index - 2;
    CPlayer *player = c->player;
    s32 n;
    char *src;
    char *dst;
    s32 i;

    if (gEngineApp_m_Settings_match_tag) {
        n = player->m_pOptions->m_Tagged[slot];
    } else {
        n = player->m_pOptions->m_Killed[slot];
    }
    dst = a->text;
    src = (char *)gEngineApp_m_Settings_slots[slot].m_Name;
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    sprintf(a->text + (MenuItem_TextLength(a) - 2), D_800C1338, n);
    return 0;
}
