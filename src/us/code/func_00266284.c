/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204620.c, func_00204620.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"

/* Turok 2: def at 0x14, flags at 0xD4 (Rage Wars 0x18 / 0x100). */
s32 func_00266284(CGameObjectInstance *actor) {
    s32 v = actor->header.m_pIntelligence->m_Action.m_GoAnim;
    if (v != 0) {
        actor->m_dwFlags |= 0x2000;
    } else {
        actor->m_dwFlags &= ~0x2000;
        actor->m_dwFlags &= ~0x100;
    }
    return v;
}
