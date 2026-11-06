#include "common.h"
#include "structs/world.h"

void CScene__ConstructWarp(CScene *arg0) {
    arg0->m_WarpFade = 2;
    arg0->m_WarpState = 0;
    arg0->m_WarpTime = 0;
    arg0->m_bWarpSpawn = 0;
    arg0->m_LevelID = 0;
    arg0->m_WarpID = 0;
    arg0->unk_18FD8 = 0;
    arg0->m_PendingWarpID = 0;
}
