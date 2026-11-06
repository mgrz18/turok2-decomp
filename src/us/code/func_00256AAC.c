#include "common.h"
#include "structs/player.h"

/* PickupCounter_Get, inlined. */
static __inline__ u8 getCounter(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->m_Data.m_SpecialPickupCount[index] >> 4;
    }
    return owner->m_Data.m_SpecialPickupCount[index] & 0xF;
}

/* Bumps one nibble of a packed counter byte and returns that nibble. */
u8 PickupCounter_Inc(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        owner->m_Data.m_SpecialPickupCount[index] += 0x10;
    } else {
        owner->m_Data.m_SpecialPickupCount[index] += 1;
    }
    return getCounter(owner, index, high);
}
