/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004055D4.c, func_004055D4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
typedef struct O01Rumble {
               u8 ok;
               u8 pad1[7];
               OSPfs pfs;
} O01Rumble;
typedef struct O01Pad4 {
            u8 pad0[3];
            u8 unk3;
} O01Pad4;
extern O01Rumble D_80130AA0[];
extern O01Pad4 D_800F6778[];
extern OSMesgQueue D_800F6798;
s32 func_0045FB34(s32 ch) {
    O01Rumble *r = &D_80130AA0[ch];
    if (D_800F6778[ch].unk3 != 0) {
        r->ok = 0;
        return 0;
    }
    r->ok = osMotorInit(&D_800F6798, &r->pfs, ch) == 0;
    return r->ok;
}
