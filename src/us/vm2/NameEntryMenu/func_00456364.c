#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

/* Name entry: append the selected character to the current player's name. */



s32 NameEntryMenu_AddChar_2(MenuItem *item, Menu *event) {
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player);

    gNameEntries[i].timer = 0.0f;
    if (gNameEntries[i].length == 8) {
        return 0;
    }
    gNameEntries[i].name[gNameEntries[i].cursor] = item->text[0];
    gNameEntries[i].cursor++;
    gNameEntries[i].length++;
    gNameEntries[i].name[gNameEntries[i].length] = '\0';
    return 0;
}
