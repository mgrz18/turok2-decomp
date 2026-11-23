#include "common.h"
#include "globals/game.h"
#include "structs/player.h"

/* De-faked in round 21 (j4): the matched draft had a dead if/else with two identical
 * arms (a permuter find); the early return for the cheat (flag 0x10) matches, so the N64 and the host share it. */
/* Whether item is a special pickup (0xFA0-0x13A0); never with cheat flag 0x10. */
s32 CPlayer__IsSpecialPickup(CPlayer *player, s32 item) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 0;
    }
    return (u32) (item - 0xFA0) < 0x401U;
}
