#include "common.h"
#include "structs/world.h"

s32 Sound__Play2D(s32);

void CScene__RequestWarp(CWorld *arg0, s32 arg1, s32 arg2) {
    arg0->m_WarpState = 1;
    arg0->m_WarpFade = arg2;
    arg0->m_WarpID = arg1;
    if ((arg2 == 2) && (arg1 != 0x3E7)) {
        Sound__Play2D(0xBE0);
    }
}
