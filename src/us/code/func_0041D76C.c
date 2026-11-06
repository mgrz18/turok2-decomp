/* WRONG CUT: real function 0x0041D76C, size 0xE0 (through 0x0041D84C): splat cut it in two
 * (NameEntryMenu_Confirm, func_0041D7B0) in the middle of an index computation. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"


extern s32 CPlayerPool__GetIndex(void *players, void *player);
extern void PlayerOptions_SetDefaultName(void *options);
extern void MenuItem_SelectSubmenu(s32 menu, Menu *c, s32 pool);

/* Name entry: confirm. Copies the name (blanks for unused characters) into the setup slot;
 * an empty name resets the player's options. */
s32 NameEntryMenu_Confirm(s32 menu, Menu *c, s32 pool) {
    CPlayer *player = c->player;
    s32 i = CPlayerPool__GetIndex(&D_80119870.m_PlayerList, player);
    u8 *src = D_800C1FB0[i].name;
    u8 *dst = D_80119870.m_Settings.slots[0].m_Name + (i << 6);
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
        PlayerOptions_SetDefaultName(player->m_pOptions);
    }
    MenuItem_SelectSubmenu(menu, c, pool);
    return 1;
}
