#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/cheat.h"

/* The cheat keyboard's labels: one-character strings a word apart (" ", "Z", "Y", ...), drawn
 * as text. Declared as bytes so that the port generates them in ROM byte order: as s32 words
 * they were swapped and every key of the Enter Cheat screen drew an empty string. */
extern u8 D_800C1144[];

s32 EnterCheatMenu_ResultLabel(MenuItem *arg0) {
    if ((u32) sEnterCheatResult < 0xCU) {
        arg0->table = (s32 *) sCheatCodes[sEnterCheatResult].unk4;
    } else {
        arg0->table = (s32 *) D_800C1144;
    }
    return 0;
}
