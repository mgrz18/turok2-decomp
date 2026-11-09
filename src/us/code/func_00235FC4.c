#include "common.h"
#include "globals/engine.h"
#include "structs/swoosh.h"

/* The ribbon vertex pool: 300 vertices for this frame's buffer. */
void RibbonVtxPool_Reset(CSwooshSystem *arg0) {
    s32 i = gEvenOdd;
    arg0->m_RibbonVtxLeft = 0x12C;
    arg0->m_pRibbonVtx = arg0->m_RibbonVtx[i];
}
