/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043C730.c, func_0043D2D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu.h"
typedef struct { u8 pad0[8]; u32 f8; u8 padC[8]; void *f14; } Q4Z;
extern u8 D_800C1144[];
extern u8 sCheatMenuText_WarpPortOfAdia[], sCheatMenuText_WarpRiverOfSouls[], sCheatMenuText_WarpDeathMarshes[], sCheatMenuText_WarpLairOfTheBlindOnes[], sCheatMenuText_WarpHiveOfTheMantids[], sCheatMenuText_WarpPrimagensLightship[], sCheatMenuText_WarpQueenBoss[], sCheatMenuText_WarpBlindOneBoss[], sCheatMenuText_WarpMotherBoss[], sCheatMenuText_WarpPrimagenBoss[], sCheatMenuText_WarpCredits[];
s32 CheatMenu_WarpText_2(Q4Z *o) {
    switch (sCheatsMenuSelection) {
    default: o->f14 = D_800C1144; break;
    case 0: o->f14 = sCheatMenuText_WarpPortOfAdia; break;
    case 1: o->f14 = sCheatMenuText_WarpRiverOfSouls; break;
    case 2: o->f14 = sCheatMenuText_WarpDeathMarshes; break;
    case 3: o->f14 = sCheatMenuText_WarpLairOfTheBlindOnes; break;
    case 4: o->f14 = sCheatMenuText_WarpHiveOfTheMantids; break;
    case 5: o->f14 = sCheatMenuText_WarpPrimagensLightship; break;
    case 6: o->f14 = sCheatMenuText_WarpBlindOneBoss; break;
    case 7: o->f14 = sCheatMenuText_WarpQueenBoss; break;
    case 8: o->f14 = sCheatMenuText_WarpMotherBoss; break;
    case 9: o->f14 = sCheatMenuText_WarpPrimagenBoss; break;
    case 10: o->f14 = sCheatMenuText_WarpCredits; break;
    }
    return 0;
}
