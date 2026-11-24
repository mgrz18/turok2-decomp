#include "common.h"
#include "protos/pak_2.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/pak.h"



void PakMenu_UnwatchAndWarp_2(s32 arg0, s32 arg1) {
    Controller_UnwatchPak(arg1);
    CScene__RequestWarp_2(&gScene, gScene_m_PendingWarpID, 2);
}
