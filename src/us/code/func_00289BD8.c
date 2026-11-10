#include "common.h"

s32 Credits_Draw(sptr);                         /* extern */
s32 Credits_Update(sptr);                         /* extern */
s32 CEngineApp__DrawWorld();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode15_Ending_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode15_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld(arg0);
    Credits_Draw(arg0);
    Credits_Update(arg0);
}
#else
void EngineMode15_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld();
    Credits_Draw(arg0);
    Credits_Update(arg0);
}
#endif
