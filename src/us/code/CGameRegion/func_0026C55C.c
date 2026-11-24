#include "common.h"
#include "structs/region.h"
#include "protos/region.h"


void CGameRegion__FloodSetFlag4(CGameRegion *arg0) {
    if ((arg0 != NULL) && (arg0->m_wFlags & 2)) {
        CGameRegion__FloodSetFlag4Same(arg0, arg0->m_nInfo);
    }
}
