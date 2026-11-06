/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043EA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu.h"
#include "structs/game.h"

extern void CEngineApp__SetupFadeTo(struct GameSession *object, int value);
extern s32 SingleMenu_GoBack(void *arg0, Outer8043E56C *arg1);
extern void Controller_UnwatchPak(s32 arg0);
s32 SingleMenu_GoBack(void *arg0, Outer8043E56C *arg1) {
    CEngineApp__SetupFadeTo(&gEngineApp, 8);
    gInSinglePlayerMenu = 0;
    Controller_UnwatchPak(arg1->unk20->unk4);
    return 1;
}
