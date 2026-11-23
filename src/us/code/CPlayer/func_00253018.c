/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_9.CPP, func_8022A170_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"
#include "globals/game.h"
#include "protos/cache.h"

#ifdef TARGET_PORT
struct ObjectPool {
/* the view at the host offsets of CPlayerPool (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ObjectPool(m) __builtin_offsetof(CPlayerPool, m)
    union {
        struct { u8 _hp_allocation[T2_HOFF_ObjectPool(m_pmePlayers)]; void *allocation; }; /* N64 0x0 */
        struct { u8 _hp_objects[T2_HOFF_ObjectPool(m_Players)]; char *objects; }; /* N64 0x4 */
        struct { u8 _hp_count[T2_HOFF_ObjectPool(m_nPlayers)]; s32 count; }; /* N64 0x8 */
        struct { u8 _hp_active_queue[T2_HOFF_ObjectPool(m_IdleList)]; CList active_queue; }; /* N64 0xC */
        struct { u8 _hp_object_queue[T2_HOFF_ObjectPool(m_ActiveList)]; CList object_queue; }; /* N64 0x20 */
    };
};
#else
struct ObjectPool {
    void *allocation;
    char *objects;
    s32 count;
    CList active_queue; /* 0x0C */
    CList object_queue; /* 0x20 */
};
#endif

/* Minimal Turok 2 view of a pooled actor (offsets differ from Rage Wars). */
#ifdef TARGET_PORT
struct Shared_ActorView {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Shared_ActorView(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_weapon[T2_HOFF_Shared_ActorView(m_WeaponActor)]; u8 weapon[sizeof(CGameObjectInstance)]; }; /* N64 0x288 */
        struct { u8 _hp_next[T2_HOFF_Shared_ActorView(m_pNext)]; struct Shared_ActorView *next; }; /* N64 0x1320 */
    };
};
#else
struct Shared_ActorView {
    u8 pad0[0x288];
    u8 weapon[0x1320 - 0x288]; /* 0x288 */
    struct Shared_ActorView *next; /* 0x1320 */
};
#endif


extern void Actor_ReloadAnimInfo(void *arg0, s32 arg1);
extern s32 CGameObjectInstance__LoadBoneMatrices(void *arg0, s32 arg1);
extern void CGameObjectInstance__ClearDrawMatrices(void *arg0);

void CPlayerPool__PreUpdate(struct ObjectPool *arg0) {
    void *node;
    if (*(void **)arg0 != 0) {
        CCache__LockForDisplayList(0, *(void **)arg0);
    }
    node = arg0->object_queue.m_pHead;
    if (node != 0) {
        u8 *base = (u8 *)&gEngineApp_m_Settings;
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
