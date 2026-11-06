#include "common.h"
#include "m2c_macros.h"

extern void func_002888BC(s32, s32, s32);
extern s32 PakMenu_PickDefaultPort(void);
extern s32 PakMenu_HasPort(void);
extern s32 CScene__LoadLevel(s32, s32, s32);
extern s32 D_800B6D64;
extern s32 D_800C2028;
extern u8 gScene[];
extern s32 D_8011B118;
extern s32 D_8011F13C;

void EngineMode00_PakCheck_Init(s32 arg0) {
    D_800C2028 = -1;
    D_8011B118 = 0;
    func_002888BC(arg0, 0, 0);
    CScene__LoadLevel((s32)gScene, 0, 0);
    PakMenu_PickDefaultPort();
    D_8011F13C = PakMenu_HasPort();
    D_800B6D64 = 1;
}
