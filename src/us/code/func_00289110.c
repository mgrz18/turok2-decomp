#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

extern void func_002888BC(sptr, s32, s32);
extern s32 PakMenu_PickDefaultPort(void);
extern s32 PakMenu_HasPort(void);

void EngineMode00_PakCheck_Init(sptr arg0) {
    gPakMenuPort = -1;
    gPakCheckTimedOut = 0;
    func_002888BC(arg0, 0, 0);
    CScene__LoadLevel(&gScene, 0, 0);
    PakMenu_PickDefaultPort();
    gPakCheckHasPak = PakMenu_HasPort();
    gPakCheckStarted = 1;
}
