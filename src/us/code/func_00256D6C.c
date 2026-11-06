#include "common.h"
#include "structs/player.h"

s32 CPlayer__IsClimbing(CPlayer *arg0) {
    return arg0->m_Mode == 0xE;
}
