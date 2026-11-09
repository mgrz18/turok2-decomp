#include "common.h"
#include "structs/game_object.h"
#include "globals/sysrc.h"

s32 CHeap__Free(CHeap *, s32);
s32 CCache__FreeMemEntry(s32, s32);
s32 CCache__DetachCacheEntry(s32, s32);


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
