#include "common.h"
#include "m2c_macros.h"

extern void CEngineApp__NewGame(sptr, s32, s32);

void EngineMode03_ExpansionPak_Init(sptr arg0) {
    CEngineApp__NewGame(arg0, 0, 0);
}
