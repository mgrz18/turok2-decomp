#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

#ifdef TARGET_PORT
extern void ImpactEvent_FloodRegionFlag4(sptr, CVector3, s32);

/* impact event 7 (a door closing): the actor pointer at full width */
void ImpactEvent_DoorClose(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) {
#else
extern void ImpactEvent_FloodRegionFlag4(s32, CVector3, s32);

void ImpactEvent_DoorClose(s32 arg0, s32 arg1, s32 arg2, CVector3 arg3) {
#endif
    ImpactEvent_FloodRegionFlag4(arg0, arg3, 0);
}
