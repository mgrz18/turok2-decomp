/* WRONG CUT: REAL CScene__ConstructMap_2 size 0x174 (splat cut it into CScene__ConstructMap_2 + func_0042CEB0; the word after it, a nop, is padding). */
#include "common.h"
#include "globals/strings.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/world.h"

void Model_DecompressCallback(sptr arg0, sptr arg1);

extern s32 CScene__FindObjectType(struct CScene *, s32);
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__FindObjectType_call CScene__FindObjectType
#else
s32 CScene__FindObjectType_call(void *level, s32 key) __asm__("CScene__FindObjectType");
#endif
extern s32 CScene__GetObject(struct CScene *, s32);
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__GetObject_call CScene__GetObject
#else
s32 CScene__GetObject_call(struct CScene *, s32) __asm__("CScene__GetObject");
#endif

/* Looks key up in the level and loads its record into out (out->valid = 0 when there is none). */
void CScene__ConstructMap_2(void *level, CSceneMapRecord *out, s32 key) {
    s32 id;
    s32 **h1;
    s32 **h2;
    s32 size;
    s32 res;

    id = CScene__FindObjectType_call(level, key);
    if (id == -1) {
        out->valid = 0;
        return;
    }
    res = CScene__GetObject_call(level, id);
    h1 = CCache__Request(0, res, 0x18, 0, 0, 0, sCSceneCacheDesc_ObjectIndex, 1);
    if (h1 != NULL) {
        h2 = CCache__Request(0, CISet__GetROMAddressAndSize(*h1, res, 1, &size), size, 0, 0, Model_DecompressCallback, sCSceneCacheDesc_Geometry, 1);
        if (h2 != NULL) {
            *out = *(CSceneMapRecord *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*h2, 2), 0), 0), 0);
            CCache__Deallocate(0, h2);
        }
        CCache__Deallocate(0, h1);
    }
}
