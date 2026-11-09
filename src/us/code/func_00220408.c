/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285F58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/player.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "protos/scene.h"



s32 CScene__GetInstanceSavedBit(void *arg0, void *arg1) {
    CScene *world = arg0;
    CGameObjectInstance *actor = arg1;
    void *field80;
    void *temp_v0;
    u8 *base;
    s32 field1B40C;
    s32 mask;
    sptr index;
    sptr i;
    s32 lastOffset;
    uptr first;
    if (!(actor->m_dwFlags & 0x80000)) {
        first = ((uptr)world->m_pActors);
        index = -1;
        if ((uptr) arg1 >= first) {
            lastOffset = world->m_nActors * (s32)sizeof(CGameObjectInstance);
            lastOffset -= (s32)sizeof(CGameObjectInstance);
            if (first + lastOffset >= (uptr) arg1) {
                index = ((uptr) arg1 - first) / (s32)sizeof(CGameObjectInstance);
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
    CISet__GetBlockSize(temp_v0, 1);
    base = (u8 *) CISet__GetBlock(temp_v0, 1);
    mask = 1 << (index & 7);
    i = index;
    if (i < 0) {
        i += 7;
    }
    return (base[i >> 3] & mask) != 0;
}
