/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C.CPP, func_8028B670_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/world.h"

/* Turok 2 level actor: 0x288 bytes (0x2E8 in Rage Wars). */

void *CISet__GetBlock(void *table, s32 index);
extern void CISet__GetBlockSize(s32 arg0, s32 arg1);

void CScene__SetInstanceSavedBitInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *entry;
    CIntelligence *cond;
    void *field80;
    void *temp_v0;
    u8 *base1;
    u8 *base2;
    s32 mask;
    s32 idx;
    s32 idx2;
    if (((CScene *)arg0)->m_Mode == 2) {
        return;
    }
    if (((CScene *)arg0)->m_nLevel == arg1) {
        entry = (char *)((CScene *)arg0)->m_pActors + (s32) arg2 * 0x288;
        cond = ((CGameObjectInstance *)entry)->header.m_pIntelligence;
        if ((u32) (cond->m_Type - 9) < 2) {
            if (cond->m_dwFlags & 0x200) {
                return;
            }
        }
    }
    field80 = ((CScene *)arg0)->m_pBitSections;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(field80, 0), (s32) arg1), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize((s32) temp_v0, 1);
    base1 = (u8 *) CISet__GetBlock(temp_v0, 1);
    base2 = base1;
    mask = 1 << (arg2 & 7);
    if (arg3 != 0) {
        idx = arg2;
        if ((s32) arg2 < 0) {
            idx = arg2 + 7;
        }
        base1[idx >> 3] |= mask;
        return;
    }
    idx2 = arg2;
    if (idx2 < 0) {
        idx2 += 7;
    }
    base2[idx2 >> 3] &= ~mask;
}
