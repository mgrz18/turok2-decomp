#include "common.h"
#include "globals/render.h"

s32 CEngineApp__DrawWorld();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode04_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode04_Cinema_Draw(sptr pt0) {
    gFogEnabled = 0;
    CEngineApp__DrawWorld(pt0);
}
#else
void EngineMode04_Cinema_Draw(void) {
    gFogEnabled = 0;
    CEngineApp__DrawWorld();
}
#endif
