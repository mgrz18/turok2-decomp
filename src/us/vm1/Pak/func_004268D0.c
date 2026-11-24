/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_80405290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pak.h (round 29, part r2: gPakFileSystems as OSPfs, the host's size too). */

#include "common.h"
#include "ultra_types.h"
#include "globals/pak.h"

extern s32 Controller_Lock(s32 arg0);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
void Pak_Mount(s32);
s32 osPfsRepairId(OSPfs *pfs);
s32 Pak_Repair(s32 arg0) {
    s32 var_s0;
    if (gPakState[arg0] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    gPakQueueState = 2;
    var_s0 = osPfsRepairId(&gPakFileSystems[arg0]);
    if (var_s0 != 0) {
        var_s0 = -1;
    }
    Pak_Mount(arg0);
    Controller_Unlock();
    return var_s0;
}
