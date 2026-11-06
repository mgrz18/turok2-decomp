/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442A58_de_3.c, func_80444314_eu.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 D_80130A34;
extern s32 D_80130A30;
extern s32 D_80110048;
extern s32 Level_GetHub(s32);
void MissionMenu_InitFailed(void) {
    D_80130A30 = Level_GetHub(D_80110048);
    D_80130A34 = 1;
}
