#include "common.h"
#include "globals/strings.h"
#include "structs/game_object.h"
#include "protos/iset.h"
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
s32 CGameObjectInstance__BuildBoneMatrices(void *, void *, sptr);

s32 CGameObjectInstance__LoadBoneMatrices(CGameObjectInstance *obj, s32 arg1) {
    s32 result;
    void **res;
    void **sub;
    void *first;

    result = 0;
    if (!(obj->m_dwFlags & 0x40000)) {
        return result;
    }
    res = CCache__Request(0, (sptr)obj->m_rpObject, obj->m_rpObjectHeader, 0, 0, 0, sCGameObjectInstanceCacheDesc_AnimObjectInfo, arg1);
    if (res != NULL) {
        sub = CGameObjectInstance__LoadGeometry_call(obj);
        if (sub != NULL) {
            first = *sub;
            result = CGameObjectInstance__BuildBoneMatrices(obj, first, CISet__GetBlock(*res, 0));
            CCache__Unlock(0, sub);
        }
        CCache__Unlock(0, res);
    }
    return result;
}
