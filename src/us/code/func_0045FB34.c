/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004055D4.c, func_004055D4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"

s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
s32 Pak_ProbeRumble_2(s32 ch) {
    RumblePak *r = &gRumblePaks[ch];
    if (gContStatus[ch].errno != 0) {
        r->present = 0;
        return 0;
    }
    r->present = osMotorInit(&gSiMessageQueue, &r->pfs, ch) == 0;
    return r->present;
}
