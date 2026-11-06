/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_804051D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 Controller_Lock(s32 arg0);
extern int gPakState[]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern int gPakError[]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s8 gPakQueueState;
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
void Controller_ReadAll();
void Controller_Unlock();
void Pak_Mount(s32);
s32 osPfsRepairId(void *);
s32 Pak_RepairIfDamaged(s32 arg0) {
    extern char gPakFileSystems[]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
    s32 temp_s0;
    s32 var_s0;
    temp_s0 = arg0 * 4;
    if (gPakState[arg0] != 3) {
        return -2;
    }
    ((void (*)(s32))Controller_Lock)(1);
    Controller_ReadAll();
    var_s0 = gPakError[arg0];
    gPakQueueState = 2;
    if (var_s0 == -4) {
        var_s0 = osPfsRepairId((arg0 * 0x68) + gPakFileSystems);
        if (var_s0 != 0) {
            var_s0 = -1;
        }
        Pak_Mount(arg0);
    }
    Controller_Unlock();
    return var_s0;
}
