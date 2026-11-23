#include "common.h"

extern void CPlayer__UpdateDeath();

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath28_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath28_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}
