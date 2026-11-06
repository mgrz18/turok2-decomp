/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_8.CPP, func_80405290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern s32 Controller_Lock(s32 arg0);
extern s8 gPakQueueState;
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
void Controller_ReadAll();
void Controller_Unlock();
void Pak_Mount_2(s32);
s32 osPfsRepairId_2(void *);
s32 Pak_Repair_2(s32 arg0) {
    s32 var_s0;
    if (gPakState[arg0] != 3) {
        return -2;
    }
    ((void (*)(s32))Controller_Lock)(1);
    Controller_ReadAll();
    gPakQueueState = 2;
    var_s0 = osPfsRepairId_2(&gPakFileSystems[arg0]);
    if (var_s0 != 0) {
        var_s0 = -1;
    }
    Pak_Mount_2(arg0);
    Controller_Unlock();
    return var_s0;
}
