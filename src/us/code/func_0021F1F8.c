#include "common.h"
#include "protos/cache.h"

#ifdef TARGET_PORT /* unprototyped: the host passes the pass-through arguments */
extern s32 **CGameObjectInstance__LoadGeometry();
#else
extern s32 **CGameObjectInstance__LoadGeometry(void);
#endif
extern u8 *CISet__GetBlock(void *base, s32 index);

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__LoadGeometry receives func_0021F1F8's own argument
 * registers (pass-through); the host passes them. */
s32 func_0021F1F8(void * pt0) {
    s32 **res;
    s32 value;

    value = -1;
    res = CGameObjectInstance__LoadGeometry(pt0);
    if (res != NULL) {
        value = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*res, 3), 0), 1)[0x66];
        CCache__Unlock(0, res);
    }
    return value;
}
#else
s32 func_0021F1F8(void) {
    s32 **res;
    s32 value;

    value = -1;
    res = CGameObjectInstance__LoadGeometry();
    if (res != NULL) {
        value = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*res, 3), 0), 1)[0x66];
        CCache__Unlock(0, res);
    }
    return value;
}
#endif
