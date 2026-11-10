#include "common.h"

s32 Hud_DrawMatchClock();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Hud_DrawMatchClock receives CPlayerPool__DrawMatchClock's own argument
 * registers (pass-through); the host passes them. */
void CPlayerPool__DrawMatchClock(s32 pt0, void * pt1) {
    Hud_DrawMatchClock(pt0, pt1);
}
#else
void CPlayerPool__DrawMatchClock(void) {
    Hud_DrawMatchClock();
}
#endif
