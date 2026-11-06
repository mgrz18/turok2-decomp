#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

extern s32 CPlayerPool__GetIndex(void *players, void *player);

/* Name entry: append the item's character, up to 8. */
s32 NameEntryMenu_AddChar(MenuItem *a, Menu *c) {
    s32 i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player);

    gNameEntries[i].timer = 0;
    if (gNameEntries[i].length == 8) {
        return 0;
    }
    gNameEntries[i].name[gNameEntries[i].cursor] = a->text[0];
    gNameEntries[i].cursor++;
    gNameEntries[i].length++;
    gNameEntries[i].name[gNameEntries[i].length] = 0;
    return 0;
}
