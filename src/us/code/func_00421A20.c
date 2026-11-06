#include "common.h"
#include "globals/scene.h"

void Controller_UnwatchPak(s32);
s32 CScene__RequestWarp(s32, s32, s32);

extern s32 gScene_m_PendingWarpID;

void func_00421A20(s32 arg0, s32 arg1) {
    Controller_UnwatchPak(arg1);
    CScene__RequestWarp((s32) &gScene, gScene_m_PendingWarpID, 2);
}
