#include "common.h"
#include "protos/ai.h"

s32 Turret_Aim();                                /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Turret_Aim receives Turret_FireUpdate's own argument
 * registers (pass-through); the host passes them. */
void Turret_FireUpdate(s32 arg0, s32 arg1) {
    if (Turret_Aim(arg0, arg1) == 0) {
        CAI__SetMode(arg0, arg1, 0x3E);
    }
}
#else
void Turret_FireUpdate(s32 arg0, s32 arg1) {
    if (Turret_Aim() == 0) {
        CAI__SetMode(arg0, arg1, 0x3E);
    }
}
#endif
