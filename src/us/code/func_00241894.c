#include "common.h"
#include "structs/collision.h"

extern CCollisionResults *gCollisionResultsP;

/* Clears the collision results: nothing hit yet. */
void CCollisionResults__Clear(void) {
    CCollisionResults *p = gCollisionResultsP;

    p->m_pInstance = 0;
    p->m_nInstanceNode = -1;
    p->m_pShape = 0;
    p->m_pWallRegion = 0;
    p->m_nFloorKind = 0;
    p->m_pFloorRegion = 0;
    p->m_pWaterRegion = 0;
}
