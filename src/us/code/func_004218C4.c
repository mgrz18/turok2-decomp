/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409E10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 Controller_PakChanged(s32);
extern void Controller_UpdatePakInsertion();
extern s32 D_8011F148;
extern s32 D_8011F13C;
extern s32 D_801309BC;
extern s32 D_8011B108;
extern s32 D_8011F144;
extern s32 D_800C2028;
extern s32 gEngineApp;
extern void CEngineApp__UpdateControllers(void *arg0);
s32 PakMenu_PollBootPak(s32 arg0) {
    extern s32 PakMenu_CheckPak(s32, s32, s32);
    if (D_8011B108 == 0) {
        CEngineApp__UpdateControllers(&gEngineApp);
        Controller_UpdatePakInsertion();
        if (D_8011B108 == 0) {
            if (D_8011F144 != 0) {
                D_8011F148 = 1;
            }
            D_8011F13C = 0;
            D_801309BC = 1;
        }
    }
    if (D_801309BC == 0) {
        if ((D_800C2028 != -1) && (Controller_PakChanged(D_800C2028) != 0) && (PakMenu_CheckPak(arg0, D_800C2028, 0) != 0)) {
            D_8011F13C = 0;
            D_801309BC = 1;
        }
    }
    return D_801309BC;
}
