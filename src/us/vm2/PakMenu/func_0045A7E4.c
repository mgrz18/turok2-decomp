/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409E10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/pak_2.h"
#include "globals/pak.h"
#include "structs/game.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"

extern void Controller_UpdatePakInsertion();
void CEngineApp__UpdateControllers(void); /* the definition takes none; the calls pass the app */
s32 PakMenu_PollBootPak_2(s32 arg0) {
    if (gPakMenuStartPressed == 0) {
        ((void (*)(void *)) CEngineApp__UpdateControllers)(&gEngineApp);
        Controller_UpdatePakInsertion();
        if (gPakMenuStartPressed == 0) {
            if (gPakBootPending != 0) {
                gPakCheckDone = 1;
            }
            gPakCheckHasPak = 0;
            gPakMenuLeaving = 1;
        }
    }
    if (gPakMenuLeaving == 0) {
        if ((gPakMenuPort != -1) && (Controller_PakChanged(gPakMenuPort) != 0) && (PakMenu_CheckPak_2((struct Menu *) arg0, gPakMenuPort, 0) != 0)) {
            gPakCheckHasPak = 0;
            gPakMenuLeaving = 1;
        }
    }
    return gPakMenuLeaving;
}
