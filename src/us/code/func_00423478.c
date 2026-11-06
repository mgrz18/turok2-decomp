/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_2.CPP, func_8040B700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern s32 gPakMenuPort;
extern s32 PakMenu_CheckPak(Menu *action, s32 channel, s32 arg2);
extern s32 PakMenu_CheckPak(Menu *, s32, s32);
extern s32 Controller_PakChanged(s32);
extern s32 PakMenu_CheckPak(struct Menu *, s32, s32);
static __inline__ s32 func_80409BDC_de(struct Menu *record) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return record->controller->port;
}
s32 DamagedPakMenu_Poll(struct Menu *record) {
    if (sDamagedPakDone == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (Controller_PakChanged(func_80409BDC_de(record)) != 0) {
                if (PakMenu_CheckPak(record, func_80409BDC_de(record), 0) != 0) {
                    sDamagedPakDone = 1;
                }
            }
        }
    }
    return sDamagedPakDone;
}
