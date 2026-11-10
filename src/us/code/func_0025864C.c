#include "common.h"

void CAI__SetMode();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CAI__SetMode receives WeaponActor_SetModeWrapper's own argument
 * registers (pass-through); the host passes them. */
void WeaponActor_SetModeWrapper(void * pt0, void * pt1, s32 pt2) {
    CAI__SetMode(pt0, pt1, pt2);
}
#else
void WeaponActor_SetModeWrapper(void) {
    CAI__SetMode();
}
#endif
