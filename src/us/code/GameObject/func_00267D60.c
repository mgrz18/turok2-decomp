/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: 5 of 13 words (the permuter finds only other
 * fakes: a do { return } while (0)). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DD58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 func_00267D60(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        if (def) {
            return def->m_dwTypeFlags & 0x40;
        } else {
            return def->m_dwTypeFlags & 0x40;
        }
    }
    return 0;
}
