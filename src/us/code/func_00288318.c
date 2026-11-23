#include "common.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/engine.h"

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
