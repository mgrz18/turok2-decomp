/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_2.CPP, func_8040B700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "protos/pak_2.h"
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
s32 DamagedPakMenu_Poll_2(struct Menu *record) {
    if (sDamagedPakDone == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (Controller_PakChanged(func_80409BDC_de(record)) != 0) {
                if (PakMenu_CheckPak_2((struct Menu *) record, func_80409BDC_de(record), 0) != 0) {
                    sDamagedPakDone = 1;
                }
            }
        }
    }
    return sDamagedPakDone;
}
