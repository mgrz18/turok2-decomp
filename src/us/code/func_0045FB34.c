/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004055D4.c, func_004055D4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"
#include "structs/controller.h"

s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
extern OSMesgQueue D_800F6798;
s32 func_0045FB34(s32 ch) {
    RumblePak *r = &D_80130AA0[ch];
    if (D_800F6778[ch].errno != 0) {
        r->present = 0;
        return 0;
    }
    r->present = osMotorInit(&D_800F6798, &r->pfs, ch) == 0;
    return r->present;
}
