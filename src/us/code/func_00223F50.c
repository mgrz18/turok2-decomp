#include "common.h"
#include "structs/world.h"

void CISet__GetBlock(s32);                            /* extern */

void CScene__GetSwoosh(CScene *arg0) {
    CISet__GetBlock((s32)arg0->m_pSwooshes);
}
