/* WRONG CUT: real function 0x0041D6D4, size 0x98 (through 0x0041D76C): splat cut it in two
 * (NameEntryMenu_DeleteChar, func_0041D740) between a lui and its store. */
#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"


/* Name entry: delete the last character. */
s32 NameEntryMenu_DeleteChar(s32 menu, Menu *c) {
    s32 i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player);

    gNameEntries[i].timer = 0;
    if (gNameEntries[i].length == 0) {
        return 0;
    }
    gNameEntries[i].length--;
    gNameEntries[i].cursor--;
    gNameEntries[i].name[gNameEntries[i].cursor] = 0;
    return 0;
}
