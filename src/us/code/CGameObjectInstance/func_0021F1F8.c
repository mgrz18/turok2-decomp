#include "common.h"
#include "protos/cache.h"

/* A call to CGameObjectInstance__LoadGeometry (func_0021F358.c), not its inline expansion: in the merged
 * file the definition is DEFERRED (an inline function output at the end of the object, its string at
 * its definition), so the N64 calls it by another C name for the same symbol. */
struct CGameObjectInstance;
#ifdef TARGET_PORT
void *CGameObjectInstance__LoadGeometry(struct CGameObjectInstance *arg0);
#define CGameObjectInstance__LoadGeometry_call(obj) ((void **) CGameObjectInstance__LoadGeometry(obj))
#else
void **CGameObjectInstance__LoadGeometry_call(void *obj) __asm__("CGameObjectInstance__LoadGeometry");
#endif
#include "protos/iset.h"

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__LoadGeometry receives CGameObjectInstance__GetGeometryKind's own argument
 * registers (pass-through); the host passes them. */
s32 CGameObjectInstance__GetGeometryKind(void * pt0) {
    s32 **res;
    s32 value;

    value = -1;
    res = (s32 **) CGameObjectInstance__LoadGeometry_call(pt0);
    if (res != NULL) {
        value = ((u8 *) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*res, 3), 0), 1))[0x66];
        CCache__Unlock(0, res);
    }
    return value;
}
