/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285DB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "protos/scene.h"

/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */


/* Set or clear the actor's bit in the current level's saved-actor bitmap. */
void CScene__SetInstanceSavedBit(CScene *arg0, void *arg1, s32 arg2) {
    CGameObjectInstance *entry;
    CIntelligence *cond;
    void *field80;
    void *temp_v0;
    u8 *base1;
    u8 *base2;
    s32 level;
    s32 mask;
    sptr index;
    sptr idx;
    sptr idx2;
    s32 lastOffset;
    uptr first;
    if (!(((CGameObjectInstance *)arg1)->m_dwFlags & 0x80000)) {
        first = (uptr)arg0->m_pActors;
        index = -1;
        if ((uptr) arg1 >= first) {
            lastOffset = arg0->m_nActors * 0x288;
            lastOffset -= 0x288;
            if (first + lastOffset >= (uptr) arg1) {
                index = ((uptr) arg1 - first) / 0x288;
            }
        }
    } else {
        index = -1;
    }
    if (index == -1) {
        return;
    }
    level = arg0->m_nLevel;
    if (arg0->m_Mode == 2) {
        return;
    }
    entry = (CGameObjectInstance *)&arg0->m_pActors[index];
    cond = entry->header.m_pIntelligence;
    if ((u32) (cond->m_Type - 9) < 2) {
        if (cond->m_dwFlags & 0x200) {
            return;
        }
    }
    field80 = arg0->m_pBitSections;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(field80, 0), level), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    base1 = (u8 *) CISet__GetBlock(temp_v0, 1);
    base2 = base1;
    mask = 1 << (index & 7);
    if (arg2 != 0) {
        idx = index;
        if (index < 0) {
            idx = index + 7;
        }
        base1[idx >> 3] |= mask;
        return;
    }
    idx2 = index;
    if (idx2 < 0) {
        idx2 += 7;
    }
    base2[idx2 >> 3] &= ~mask;
}
