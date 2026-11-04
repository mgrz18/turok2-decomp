#include "common.h"
#include "structs/game_object.h"

/* Nonzero for an object (kind 1) with flags 0x300000 set. */
s32 func_0023F034(s32 arg0, CGameObjectInstance *arg1) {
    s32 r = 0;
    if (arg1->header.m_Type == 1) {
        r = (u32) r < (u32) (arg1->m_dwFlags & 0x300000);
    }
    return r;
}
