/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028CF7C.c, func_0028CF7C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

typedef struct { s32 key; u8 pad4[8]; s16 unkC; } Ent;
Ent *func_002017D4(s32 *tbl, s32 idx);
Ent *func_00225EBC(CWorld *lv, s32 a, s32 b) {
    s32 n = lv->m_pIntelligence[0];
    s32 i = 0;
    Ent *e;
    if (n > 0) {
        do {
            e = func_002017D4(lv->m_pIntelligence, i);
            if (a == -1 || e->key == a) {
                if (b == -1 || e->unkC == b) {
                    return e;
                }
            }
            i++;
        } while (i < n);
    }
    return 0;
}
