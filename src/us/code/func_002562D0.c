#include "common.h"

void CPlayer__UpdateDeath();                            /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath46_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath46_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}
