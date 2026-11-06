/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_4.CPP, func_8028C26C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/world.h"

/* Turok 2 layouts (Rage Wars offsets in comments). */

extern s32 BinaryRange(s32 *index, s32 count, s32 key, s32 *first, s32 *last);

/* Index of actor in the world's actor array, or -1 (same as CScene__GetInstanceIndex). */
static __inline__ s32 World_ActorIndex(CWorld *world, CGameObjectInstance *actor) {
    CGameObjectInstance *base;
    if (actor->m_dwFlags & 0x80000) {
        return -1;
    }
    base = (CGameObjectInstance *)world->m_pActors;
    if (actor >= base &&
        (char *)actor <= (char *)base + ((u32)world->m_nActors * sizeof(CGameObjectInstance) - sizeof(CGameObjectInstance))) {
        return ((u32)actor - (u32)base) / sizeof(CGameObjectInstance);
    }
    return -1;
}

/* Look up the run of records belonging to actor arg1: *arg2 = first record,
 * *arg3 = record count (0 if none). */
void CScene__GetInstanceEventRecords(CWorld *arg0, CGameObjectInstance *arg1, CWorldRecord **arg2, s32 *arg3) {
    s32 first;
    s32 last;
    s32 *recordIndex;
    s32 recordCount;
    s32 index = World_ActorIndex(arg0, arg1);
    if (index == -1) {
        *arg3 = 0;
        return;
    }
    recordIndex = arg0->m_pRecordIndex;
    recordCount = arg0->m_nRecords;
    if (BinaryRange(recordIndex, recordCount, index, &first, &last) != 0) {
        *arg2 = &arg0->m_pRecords[first];
        *arg3 = (last - first) + 1;
    } else {
        *arg2 = 0;
        *arg3 = 0;
    }
}
