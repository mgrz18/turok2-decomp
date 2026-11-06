/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_9.CPP, func_8022A170_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"

struct ObjectPool {
    void *allocation;
    char *objects;
    s32 count;
    CList active_queue; /* 0x0C */
    CList object_queue; /* 0x20 */
};

/* Minimal Turok 2 view of a pooled actor (offsets differ from Rage Wars). */
struct Shared_ActorView {
    u8 pad0[0x288];
    u8 weapon[0x1320 - 0x288]; /* 0x288 */
    struct Shared_ActorView *next; /* 0x1320 */
};

extern u8 D_8011AAD8[]; /* Shared_Game */

extern void Actor_ReloadAnimInfo(void *arg0, s32 arg1);
extern s32 CGameObjectInstance__LoadBoneMatrices(void *arg0, s32 arg1);
extern void CCache__LockForDisplayList(s32 arg0, void *arg1);
extern void CGameObjectInstance__ClearDrawMatrices(void *arg0);

void CPlayerPool__PreUpdate(struct ObjectPool *arg0) {
    void *node;
    if (*(void **)arg0 != 0) {
        CCache__LockForDisplayList(0, *(void **)arg0);
    }
    node = arg0->object_queue.m_pHead;
    if (node != 0) {
        u8 *base = D_8011AAD8;
        do {
            CGameObjectInstance__ClearDrawMatrices(node);
            CGameObjectInstance__ClearDrawMatrices(&((struct Shared_ActorView *)(node))->weapon);
            Actor_ReloadAnimInfo(node, 0);
            if (base[0x19] != 0) {
                CGameObjectInstance__LoadBoneMatrices(node, 1);
            }
            node = ((struct Shared_ActorView *)(node))->next;
        } while (node != 0);
    }
}
