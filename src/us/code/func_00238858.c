#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

extern void ImpactEvent_FloodRegionFlag4(sptr, CVector3, s32);

/* impact event 7 (a door closing): the actor pointer at full width */
void ImpactEvent_DoorClose(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) {
    ImpactEvent_FloodRegionFlag4(arg0, arg3, 0);
}
