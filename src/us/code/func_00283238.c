/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_7.CPP, func_8023B3F8_de, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/camera.h"
#include "structs/emitter.h"
#include "globals/frame.h"


void Render_SetRenderMode(s32 mode);
void Sort_Quick(void *items, s32 count, s32 size, void *compare, void *swap);
s32 Emitter_CompareHeightDesc(void *a, void *b);
s32 Emitter_CompareHeightAsc(void *a, void *b);
void Emitter_SwapPointers(void *a, void *b);
void CEmitter__Draw(CEmitter *e, CCamera *view);

/* Draws the running emitters: sets up the display list, splits them into
 * those deeper than the view and the others (four of each at most), sorts
 * each group and draws the deeper group first. */
void CEmitterPool__Draw(CEmitterPool *owner, CCamera *view) {
    CEmitter *near[4];
    CEmitter *far[4];
    CEmitter *e;
    s32 nearCount;
    s32 farCount;
    s32 i;

    if (owner->m_ActiveList.m_nCount == 0) {
        return;
    }
    GFX_CMD(0xE7000000, 0)
    GFX_CMD(0xDA380003, (u32)&gIdentityMtx)
    GFX_CMD(0xDB040004, 2)
    GFX_CMD(0xDB04000C, 2)
    GFX_CMD(0xDB040014, 0xFFFE)
    GFX_CMD(0xDB04001C, 0xFFFE)
    GFX_CMD(0xD9FEFFFF, 0)
    GFX_CMD(0xE3000A01, 0)
    Render_SetRenderMode(0xD);
    GFX_CMD(0xD9F9FBFF, 0)
    GFX_CMD(0xD9FFFFFF, 0x200004)
    GFX_CMD(0xE3000C00, 0x80000)
    GFX_CMD(0xE3001201, 0x2000)
    nearCount = 0;
    farCount = 0;
    for (e = owner->m_ActiveList.m_pHead; e != NULL; e = e->m_pNext) {
        if (e->m_Height > view->m_vPos.y) {
            near[nearCount++] = e;
        } else {
            far[farCount++] = e;
        }
    }
    Sort_Quick(near, nearCount, 4, Emitter_CompareHeightDesc, Emitter_SwapPointers);
    Sort_Quick(far, farCount, 4, Emitter_CompareHeightAsc, Emitter_SwapPointers);
    for (i = 0; i < nearCount; i++) {
        CEmitter__Draw(near[i], view);
    }
    for (i = 0; i < farCount; i++) {
        CEmitter__Draw(far[i], view);
    }
}
