/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_2.CPP, func_8040B700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"

static __inline__ s32 func_80409BDC_de(struct Menu *record) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return record->controller->port;
}
s32 PakMenu_PollPak(struct Menu *record) {
    if (gPakMenuLeaving == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (Controller_PakChanged(func_80409BDC_de(record)) != 0) {
                if (PakMenu_CheckPak((struct Menu *) record, func_80409BDC_de(record), 0) != 0) {
                    gPakMenuLeaving = 1;
                }
            }
        }
    }
    return gPakMenuLeaving;
}
