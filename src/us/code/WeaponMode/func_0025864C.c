#include "common.h"
#include "protos/ai.h"


/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CAI__SetMode receives WeaponActor_SetModeWrapper's own argument
 * registers (pass-through); the host passes them. */
void WeaponActor_SetModeWrapper(void * pt0, void * pt1, s32 pt2) {
    CAI__SetMode(pt0, pt1, pt2);
}
