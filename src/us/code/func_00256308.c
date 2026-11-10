#include "common.h"

s32 CPlayer__UpdateDeath();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath3C_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath3C_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}
#else
void PlayerDeath3C_Update(void) {
    CPlayer__UpdateDeath();
}
#endif
