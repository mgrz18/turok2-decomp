/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285F58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/world.h"


void *CISet__GetBlock(void *table, s32 index);
extern void CISet__GetBlockSize(s32 arg0, s32 arg1);

s32 CScene__GetInstanceSavedBit(void *arg0, void *arg1) {
    CScene *world = arg0;
    CGameObjectInstance *actor = arg1;
    void *field80;
    void *temp_v0;
    u8 *base;
    s32 field1B40C;
    s32 mask;
    s32 index;
    s32 i;
    s32 lastOffset;
    u32 first;
    if (!(actor->m_dwFlags & 0x80000)) {
        first = ((u32)world->m_pActors);
        index = -1;
        if ((u32) arg1 >= first) {
            lastOffset = world->m_nActors * 0x288;
            lastOffset -= 0x288;
            if (first + lastOffset >= (u32) arg1) {
                index = ((u32) arg1 - first) / 0x288;
            }
        }
    } else {
        index = -1;
    }
    if (index == -1) {
        return 0;
    }
    field80 = world->m_pBitSections;
    field1B40C = world->m_nLevel;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(field80, 0), field1B40C), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize((s32) temp_v0, 1);
    base = (u8 *) CISet__GetBlock(temp_v0, 1);
    mask = 1 << (index & 7);
    i = index;
    if (i < 0) {
        i += 7;
    }
    return (base[i >> 3] & mask) != 0;
}
