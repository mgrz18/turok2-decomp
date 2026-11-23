#include "common.h"

s32 CPlayer__UpdateDeath();                            /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath1E_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath1E_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}
