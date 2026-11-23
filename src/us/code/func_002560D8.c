#include "common.h"

s32 CPlayer__UpdateDeath();                            /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath32_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath32_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}
