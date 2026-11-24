#include "common.h"

s32 CEngineApp__DrawWorld();                            /* extern */
extern void func_00289108(void);

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode05_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode05_Cinema_Draw(sptr arg0) {
    CEngineApp__DrawWorld(arg0);
    ((void (*)(sptr)) func_00289108)(arg0);
}
