/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B1F8.c, func_0028B1F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

s32 CScene__FindObjectType(CScene *lv, s32 id) {
    IdTable *t = lv->m_pObjectTypes;
    s32 n = t->count;
    u16 *p = t->ids;
    s32 i = 0;
    if (n > 0) {
        do {
            if (*p == (u16)id) {
                return i;
            }
            i++;
            p++;
        } while (i < n);
    }
    return -1;
}
