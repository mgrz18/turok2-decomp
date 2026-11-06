/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0020459C.c, func_0020459C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/intelligence.h"


s32 Action_GetIdleAnim(CGameObjectInstance *actor) {
    s32 v = actor->header.m_pIntelligence->m_Action.m_IdleAnim;
    if (v != 0) {
        actor->m_dwFlags |= 0x2000;
    } else {
        actor->m_dwFlags &= ~0x2000;
    }
    return v;
}
