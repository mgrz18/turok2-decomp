#include "common.h"

extern void Credits_Draw(void);
void Credits_Update(sptr);                         /* extern */
void CEngineApp__DrawWorld();                            /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode16_Ending_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode16_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld(arg0);
    ((void (*)(sptr)) Credits_Draw)(arg0);
    Credits_Update(arg0);
}
