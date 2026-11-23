#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

s32 CPlayer__IsBelowFullHealth(CPlayer *arg0) {
    return arg0->m_Data.m_Health < 0x6400;
}
