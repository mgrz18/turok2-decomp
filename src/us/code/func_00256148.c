#include "common.h"

#ifdef TARGET_PORT /* unprototyped: the host passes the pass-through arguments */
extern void CPlayer__UpdateDeath();
#else
extern void CPlayer__UpdateDeath(void);
#endif

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath28_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath28_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}
#else
void PlayerDeath28_Update(void) {
    CPlayer__UpdateDeath();
}
#endif
