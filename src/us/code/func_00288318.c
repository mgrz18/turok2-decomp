#include "common.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/engine.h"

#ifdef TARGET_PORT
/* The port: the same through CEngineApp / CFrameData (structs/game.h, frame.h). */
#include "structs/game.h"
void CEngineApp__AdvanceFrameData(CEngineApp *gfx) {
    gFrameCount++;
    gFrameCountGameplay++;
    gEvenOdd ^= 1;
    gfx->m_pCurrentFrameData = &gfx->m_FrameData[gFrameCount % gTotalFramebuffers];
    gfx->m_pCurrentFrameData->m_pDisplayList = (void *)(sptr)(gEvenOdd ? gDisplayList0 : gDisplayList1);
    gfx->m_pCurrentFrameData->m_pLineList = (void *)(sptr)(gEvenOdd ? gLineList0 : gLineList1);
}
#else
typedef struct {
    /* 0x000 */ char pad0[0x114];
    /* 0x114 */ s32 colorImage;
    /* 0x118 */ s32 depthImage;
    /* 0x11C */ char pad11C[0x24];
} GfxTask288318; /* size 0x140 */

typedef struct {
    /* 0x000 */ GfxTask288318 tasks[3];
    /* 0x3C0 */ GfxTask288318 *cur;
} GfxState288318;

void CEngineApp__AdvanceFrameData(GfxState288318 *gfx) {
    gFrameCount++;
    gFrameCountGameplay++;
    gEvenOdd ^= 1;
    gfx->cur = &gfx->tasks[gFrameCount % gTotalFramebuffers];
    gfx->cur->colorImage = gEvenOdd ? gDisplayList0 : gDisplayList1;
    gfx->cur->depthImage = gEvenOdd ? gLineList0 : gLineList1;
}
#endif
