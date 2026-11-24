#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/pak.h"



void PakMenu_UnwatchAndWarp(s32 arg0, s32 arg1) {
    Controller_UnwatchPak(arg1);
    CScene__RequestWarp(&gScene, gScene_m_PendingWarpID, 2);
}
