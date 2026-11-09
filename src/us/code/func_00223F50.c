#include "common.h"
#include "structs/world.h"
#include "protos/scene.h"

void CISet__GetBlock(s32);                            /* extern */

void CScene__GetSwoosh(CScene *arg0) {
    CISet__GetBlock((sptr)arg0->m_pSwooshes);
}
