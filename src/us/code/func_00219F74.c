#include "common.h"
#include "structs/game_object.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/cache.h"

/* Pick, at random, one entry of the actor's animation set (0xBA) whose action
 * is `action`: among the run of equal entries around the one found, leaving
 * out index `skip` (-1: none). */


typedef struct {
    s16 count;      /* 0x00 */
    u8 pad[0x66];
} RandList;         /* 0x68 */

extern u8 D_800A5AC4[];     /* "anim object info" */

s32 BinarySearch(s32 *, s32, sptr);
void WeightedTable_Clear(RandList *);
void WeightedTable_Add(RandList *, s16, s32);
s32 WeightedTable_Pick(RandList *);

sptr CGameObjectInstance__PickRandomAnim(CGameObjectInstance *actor, sptr action, sptr skip) {
    RandList list;
    CMemEntry *res;
    s32 *data;
    s32 *entries;
    s32 count;
    sptr i;
    sptr found;

    if (actor->m_dwFlags & 0x40000) {
        res = CCache__Request(0, ((sptr)actor->m_rpObject), actor->m_rpObjectHeader, 0, 0, 0, D_800A5AC4, 1);
        if (res != NULL) {
            data = CISet__GetBlock(res->data, 1);
            entries = data + 2;
            count = data[1];
            i = BinarySearch(entries, actor->m_nAnims, action);
            if (i != -1) {
                found = i;
                while (i > 0 && entries[i - 1] == action) {
                    i--;
                }
                WeightedTable_Clear(&list);
                if (skip == -1) {
                    while (i < count && entries[i] == action) {
                        WeightedTable_Add(&list, i++, 10);
                    }
                } else {
                    while (i < count && entries[i] == action) {
                        if (skip != i) {
                            WeightedTable_Add(&list, i, 10);
                        }
                        i++;
                    }
                }
                if (list.count == 0) {
                    WeightedTable_Add(&list, found, 10);
                }
                i = (s16) WeightedTable_Pick(&list);
            }
            CCache__Unlock(0, res);
            return i;
        }
    }
    return -1;
}
