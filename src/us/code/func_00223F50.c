#include "common.h"
#include "structs/world.h"
#include "protos/scene.h"

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used + missing_args): the caller (func_002350EC) passes the
 * swoosh type in a1 and uses the block address; on MIPS a1 passes straight through to
 * CISet__GetBlock and its v0 is returned. */
void *CScene__GetSwoosh(CScene *arg0, s32 index) {
    return CISet__GetBlock(arg0->m_pSwooshes, index);
}
#else
void CISet__GetBlock(s32);                            /* extern */

void CScene__GetSwoosh(CScene *arg0) {
    CISet__GetBlock((sptr)arg0->m_pSwooshes);
}
#endif
