/* FAKEMATCH (round 22): the `do { return ...; } while (0)` (the permuter's other form of func_00267D60's fake);
 * the plain getter, on the typed CIntelligence too, gives the definition $a0 and the type $v1: 5 of 13 words. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DE10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 func_00267E18(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        do { return def->m_dwTypeFlags & 0x100; } while (0);
    }
    return 0;
}
