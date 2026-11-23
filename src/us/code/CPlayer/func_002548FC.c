#include "common.h"
#include "structs/player.h"

s32 CPlayer__IsInMatchEndState(CPlayer *arg0) {
    s16 temp_v1;

    temp_v1 = arg0->m_Mode;
    if ((temp_v1 == 0x12) || (temp_v1 == 0x10) || (temp_v1 == 0x11)) {
        return 1;
    }
    return 0;
}
