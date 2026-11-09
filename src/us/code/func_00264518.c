/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00207B5C.c, func_00207B5C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "protos/game.h"
void Platform_ForwardEnter(CGameObjectInstance *actor, u32 *flags) {
    CPlatformIntelligence *info = &actor->header.m_pIntelligence->m_Platform;
    GameObject_FireTriggers(actor, 0x10000, actor);
    if (!(info->m_dwPlatformFlags & 0x40)) {
        *flags &= 0xF7FFFFFF;
    }
}
