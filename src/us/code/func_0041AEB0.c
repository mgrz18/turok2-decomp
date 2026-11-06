#include "common.h"
#include "structs/menu.h"
#include "globals/cheat.h"

extern s32 D_800C1144;
extern u32 sEnterCheatResult;

s32 EnterCheatMenu_ResultLabel(MenuItem *arg0) {
    if ((u32) sEnterCheatResult < 0xCU) {
        arg0->table = (s32 *) sCheatCodes[sEnterCheatResult].unk4;
    } else {
        arg0->table = &D_800C1144;
    }
    return 0;
}
