/* Cut fixed in versions/cuts.us.txt: PickupCounter_Dec really is 0x256AF0-0x256B34 (0x44 bytes, like its twin PickupCounter_Inc): splat cut its low-nibble return off as func_00256B24 (whose C in src/ is a fake for that tail). */
#include "common.h"
#include "structs/player.h"

/* PickupCounter_Get, inlined. */
static __inline__ u8 getCounter(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->m_Data.m_SpecialPickupCount[index] >> 4;
    }
    return owner->m_Data.m_SpecialPickupCount[index] & 0xF;
}

/* Drops one nibble of a packed counter byte and returns that nibble. */
u8 PickupCounter_Dec(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        owner->m_Data.m_SpecialPickupCount[index] -= 0x10;
    } else {
        owner->m_Data.m_SpecialPickupCount[index] -= 1;
    }
    return getCounter(owner, index, high);
}
