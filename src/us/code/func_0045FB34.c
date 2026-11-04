/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004055D4.c, func_004055D4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"

s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
typedef struct O01Pad4 {
            u8 pad0[3];
            u8 unk3;
} O01Pad4;
extern O01Pad4 D_800F6778[];
extern OSMesgQueue D_800F6798;
s32 func_0045FB34(s32 ch) {
    RumblePak *r = &D_80130AA0[ch];
    if (D_800F6778[ch].unk3 != 0) {
        r->present = 0;
        return 0;
    }
    r->present = osMotorInit(&D_800F6798, &r->pfs, ch) == 0;
    return r->present;
}
