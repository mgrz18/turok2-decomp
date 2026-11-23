#include "common.h"
#include "structs/world.h"

/* The counts this clears, scattered over a large block. */

void CScene__ClearListCounts(CScene *arg0) {
    arg0->m_nDrawActors = 0;
    arg0->m_nMovers = 0;
    arg0->m_nListed = 0;
    arg0->m_nRegistered = 0;
    arg0->m_nKind1 = 0;
    arg0->m_nFlagged = 0;
    arg0->m_nKind5 = 0;
    arg0->m_nList6 = 0;
    arg0->m_nList7 = 0;
    arg0->m_nDeferred = 0;
    arg0->m_nSectors = 0;
}
