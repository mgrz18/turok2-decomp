#include "common.h"
#include "structs/player.h"

u8 PickupCounter_Get(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->m_Data.m_SpecialPickupCount[index] >> 4;
    }
    return owner->m_Data.m_SpecialPickupCount[index] & 0xF;
}
