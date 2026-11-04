/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024DEF8.c, func_0024DEF8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
s32 func_00267F10(CGameObjectInstance *a) {
    CIntelligence *d = a->header.m_pIntelligence;
    s32 r;
    switch (d->m_Type) {
    case 11:
        return 1;
    case 1:
        r = 0;
        if (a->m_AI.m_Health <= 0 || (d->m_Enemy.m_dwEnemyFlags & 2)) {
            r = 1;
        }
        return r;
    default:
        return 0;
    }
}
