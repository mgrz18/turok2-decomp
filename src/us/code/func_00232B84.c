/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/CODE_8028414C.CPP, Particle_AllocPoolNode.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/list.h"
struct PoolNode84478;
typedef struct PoolBucket84478 {
    struct PoolNode84478 *head;
    struct PoolNode84478 *tail;
    s32 data[3];
} PoolBucket84478;
typedef struct PoolOwner84478 {
    char pad0[0xEC00]; /* Turok 2 offsets (Rage Wars: 0xFC00) */
    PoolBucket84478 free;
    char padEC14[0x14];
    PoolBucket84478 slots[256];
} PoolOwner84478;
typedef struct PoolNode84478 {
    char pad0[0x40]; /* Turok 2 layout (Rage Wars: state 0x5C, owner 0x1E4) */
    s32 state;
    char pad44[0x1C0 - 0x44];
    PoolBucket84478 *owner;
    char pad1C4[8];
    s32 counter;
    s32 flags;
} PoolNode84478;
extern void CParticleSystem__Free(PoolOwner84478 *, PoolNode84478 *);
#ifdef TARGET_PORT
#include "structs/particle.h"
/* the host CParticleSystem/CParticle by member (the views are at N64 offsets) */
PoolNode84478 *CParticleSystem__AllocNode(PoolOwner84478 *owner, s32 index) {
    CParticleSystem *sys = (CParticleSystem *) owner;
    CParticle *node;
    CList *slot;
    s32 i;

    if (sys->m_FreeList.m_pHead == 0) {
        for (i = 0; i <= (index & 0xFF); i++) {
            if (sys->m_ActiveList[i].m_pTail != 0) {
                CParticleSystem__Free(owner, sys->m_ActiveList[i].m_pTail);
                break;
            }
        }
    }
    node = sys->m_FreeList.m_pHead;
    if (node == 0) {
        return 0;
    }
    CList__Remove(&sys->m_FreeList, node);
    slot = &sys->m_ActiveList[index & 0xFF];
    CList__AddHead(slot, node);
    node->m_pActiveList = slot;
    node->m_pDrawNext = 0;
    node->m_pDrawLast = 0;
    node->m_dwFlags = 0x100;
    return (PoolNode84478 *) node;
}
#else
PoolNode84478 *CParticleSystem__AllocNode(PoolOwner84478 *owner, s32 index) {
    PoolNode84478 *node;
    PoolBucket84478 *slot;
    s32 i;
    PoolNode84478 *tail;
    if (owner->free.head == 0) {
        for (i = 0; i <= (index & 0xFF); i++) {
            tail = (&owner->slots[i])->tail;
            if (tail != 0) {
                CParticleSystem__Free(owner, tail);
                break;
            }
        }
    }
    node = owner->free.head;
    if (node == 0) {
        return 0;
    }
    CList__Remove(&owner->free, node);
    slot = &owner->slots[index & 0xFF];
    CList__AddHead(slot, node);
    node->owner = slot;
    node->flags = 0;
    node->counter = 0;
    node->state = 0x100;
    return node;
}
#endif
