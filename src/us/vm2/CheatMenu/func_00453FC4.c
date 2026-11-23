/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043C730.c, func_0043D2D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"
extern u8 D_800C1144[];
s32 CheatMenu_WarpText_2(MenuItem *o) {
    switch (sCheatsMenuSelection) {
    default: o->text = (char *)D_800C1144; break;
    case 0: o->text = (char *)sCheatMenuText_WarpPortOfAdia; break;
    case 1: o->text = (char *)sCheatMenuText_WarpRiverOfSouls; break;
    case 2: o->text = (char *)sCheatMenuText_WarpDeathMarshes; break;
    case 3: o->text = (char *)sCheatMenuText_WarpLairOfTheBlindOnes; break;
    case 4: o->text = (char *)sCheatMenuText_WarpHiveOfTheMantids; break;
    case 5: o->text = (char *)sCheatMenuText_WarpPrimagensLightship; break;
    case 6: o->text = (char *)sCheatMenuText_WarpBlindOneBoss; break;
    case 7: o->text = (char *)sCheatMenuText_WarpQueenBoss; break;
    case 8: o->text = (char *)sCheatMenuText_WarpMotherBoss; break;
    case 9: o->text = (char *)sCheatMenuText_WarpPrimagenBoss; break;
    case 10: o->text = (char *)sCheatMenuText_WarpCredits; break;
    }
    return 0;
}
