#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"

extern void func_002888BC(s32, s32, s32);
extern s32 PakMenu_PickDefaultPort(void);
extern s32 PakMenu_HasPort(void);
extern s32 CScene__LoadLevel(s32, s32, s32);
extern s32 gPakCheckStarted;
extern s32 gPakMenuPort;
extern u8 gScene[];
extern s32 gPakCheckTimedOut;

void EngineMode00_PakCheck_Init(s32 arg0) {
    gPakMenuPort = -1;
    gPakCheckTimedOut = 0;
    func_002888BC(arg0, 0, 0);
    CScene__LoadLevel((s32)gScene, 0, 0);
    PakMenu_PickDefaultPort();
    gPakCheckHasPak = PakMenu_HasPort();
    gPakCheckStarted = 1;
}
