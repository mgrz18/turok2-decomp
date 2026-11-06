#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

extern s32 CPlayerPool__GetIndex(void *players, void *player);

/* Name entry: append the item's character, up to 8. */
s32 NameEntryMenu_AddChar(MenuItem *a, Menu *c) {
    s32 i = CPlayerPool__GetIndex(&D_80119870.m_PlayerList, c->player);

    D_800C1FB0[i].timer = 0;
    if (D_800C1FB0[i].length == 8) {
        return 0;
    }
    D_800C1FB0[i].name[D_800C1FB0[i].cursor] = a->text[0];
    D_800C1FB0[i].cursor++;
    D_800C1FB0[i].length++;
    D_800C1FB0[i].name[D_800C1FB0[i].length] = 0;
    return 0;
}
