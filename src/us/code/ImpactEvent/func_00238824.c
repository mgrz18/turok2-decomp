#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

extern void ImpactEvent_FloodRegionFlag4(sptr, CVector3, s32);

void ImpactEvent_DoorOpen(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) { /* impact event (a door opening): the actors pointer-wide */
    ImpactEvent_FloodRegionFlag4(arg0, arg3, 1);
}
