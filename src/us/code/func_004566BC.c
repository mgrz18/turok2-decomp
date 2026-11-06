#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

/* Name entry accepted: copy the edited name (blank-padded to 8 characters)
 * into the current player's record; an empty name restores the default one. */


s32 CPlayerPool__GetIndex(void *state, void *player);
void func_0041BBB0(void *options);
void MenuItem_SelectSubmenu(void *menu, Menu *event, void *arg2);

s32 NameEntryMenu_Confirm_2(void *menu, Menu *event, void *arg2) {
    u8 *player = event->player;
    s32 i = CPlayerPool__GetIndex(&D_80119870.m_PlayerList, player);
    u8 *src = (u8 *)D_800C1FB0[i].name;
    u8 *dst = D_80119870.m_Settings.slots[0].m_Name + i * 64;
    s32 count = 0;
    s32 k;
    u8 c;

    for (k = 0; k < 8; k++) {
        c = *src++;
        if (c == 0) {
            c = ' ';
        } else {
            count++;
        }
        *dst++ = c;
    }
    *dst = '\0';
    if (count == 0) {
        func_0041BBB0(*(void **)(player + 0x518));
    }
    MenuItem_SelectSubmenu(menu, event, arg2);
    return 1;
}
