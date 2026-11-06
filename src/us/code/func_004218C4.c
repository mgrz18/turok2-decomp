/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409E10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern s32 Controller_PakChanged(s32);
extern void Controller_UpdatePakInsertion();
extern s32 D_8011F144;
extern s32 gPakMenuPort;
extern s32 gEngineApp;
extern void CEngineApp__UpdateControllers(void *arg0);
s32 PakMenu_PollBootPak(s32 arg0) {
    extern s32 PakMenu_CheckPak(s32, s32, s32);
    if (gPakMenuStartPressed == 0) {
        CEngineApp__UpdateControllers(&gEngineApp);
        Controller_UpdatePakInsertion();
        if (gPakMenuStartPressed == 0) {
            if (D_8011F144 != 0) {
                gPakCheckDone = 1;
            }
            gPakCheckHasPak = 0;
            gPakMenuLeaving = 1;
        }
    }
    if (gPakMenuLeaving == 0) {
        if ((gPakMenuPort != -1) && (Controller_PakChanged(gPakMenuPort) != 0) && (PakMenu_CheckPak(arg0, gPakMenuPort, 0) != 0)) {
            gPakCheckHasPak = 0;
            gPakMenuLeaving = 1;
        }
    }
    return gPakMenuLeaving;
}
