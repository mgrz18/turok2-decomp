#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* An enemy definition's type flag 1 (0 for other definitions). */
s32 GameObject_IsLeader(CInstanceHeader *header) {
    CIntelligence *def = header->m_pIntelligence;

    if (def->m_Type == INTELLIGENCE_ENEMY) {
        return def->m_dwTypeFlags & 1;
    }
    return 0;
}
