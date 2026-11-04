/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024E1A4.c, func_0024E1A4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
s32 func_002681BC(CGameObjectInstance *a) {
    CAIModeInfo *p;
    if (a->header.m_Type != 1) {
        return 0;
    }
    if (!(a->m_dwFlags & 0x2000)) {
        return 0;
    }
    p = a->m_AI.m_pModeCurrent;
    if (p != 0) {
        if (p->unk_1C & 0x10000) {
            return 0;
        }
    }
    return 1;
}
