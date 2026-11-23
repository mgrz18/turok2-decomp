#include "common.h"
#include "globals/game.h"
#include "structs/player.h"

/* De-faked in round 21 (j4): the matched draft had a dead if/else with two identical
 * arms (a permuter find); the early return for the cheat (flag 0x10) matches, so the N64 and the host share it. */
/* The high nibble of the special pickup's count (1 with cheat flag 0x10). */
u32 CPlayer__GetPickupCountHigh(CPlayer *player, s32 item) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return player->m_Data.m_SpecialPickupCount[item - 0xFA0] >> 4;
}
