#include "common.h"
#include "structs/game_object.h"
#include "globals/sysrc.h"

#ifdef TARGET_PORT
#include "structs/cache.h"
s32 CHeap__Free(CHeap *, u8 *);
void CCache__FreeMemEntry(s32, CMemEntry *);
void CCache__DetachCacheEntry(s32, void *);

/* The N64 body reads the CMemEntry through a CAI view (m_dwFlags = data,
 * m_InitialHealth = lockCount, m_VisBits = flags): N64 offsets, wrong on the host. */
void CCache__DeallocateInternal(s32 arg0, CMemEntry *e) {
    e->flags &= ~2;
    if (e->lockCount != 0) {
        e->lockCount = 0;
        e->flags &= ~0x100;
    }
    if (!(e->flags & 0x702)) {
        CCache__DetachCacheEntry(0, e);
        CHeap__Free(&gHeap, e->data);
        CCache__FreeMemEntry(0, e);
    }
}
#else
s32 CHeap__Free(CHeap *, s32);
s32 CCache__FreeMemEntry(s32, sptr);
s32 CCache__DetachCacheEntry(s32, sptr);


void CCache__DeallocateInternal(s32 arg0, CAI *ai) {
    s32 temp_v0;

    ai->m_VisBits = (s32) (ai->m_VisBits & ~2);
    if (ai->m_InitialHealth != 0) {
loop_1:
        do {
            temp_v0 = ai->m_InitialHealth - 1;
            ai->m_InitialHealth = temp_v0;
            if (temp_v0 != 0) {
                goto loop_1;
            }
            ai->m_VisBits = (s32) (ai->m_VisBits & ~0x100);
        } while (ai->m_InitialHealth != 0);
    }
    if (!(ai->m_VisBits & 0x702)) {
        CCache__DetachCacheEntry(0, (sptr) ai);
        CHeap__Free(&gHeap, ai->m_dwFlags);
        CCache__FreeMemEntry(0, (sptr) ai);
    }
}
#endif
