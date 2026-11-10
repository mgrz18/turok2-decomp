#include "common.h"

void CEngineApp__DrawWorld();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode11_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode11_Cinema_Draw(sptr pt0) {
    CEngineApp__DrawWorld(pt0);
}
#else
void EngineMode11_Cinema_Draw(void) {
    CEngineApp__DrawWorld();
}
#endif
