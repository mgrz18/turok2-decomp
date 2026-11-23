/* WRONG CUT: REAL CScene__ConstructMap size 0x174 (splat cut it into CScene__ConstructMap + func_0042CEB0; the word after it, a nop, is padding). */
#include "common.h"
#include "globals/strings.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/world.h"

extern void Model_DecompressCallback(void);

extern s32 CScene__FindObjectType(void *level, s32 key);
extern s32 CScene__GetObject(void *level, s32 id);

/* Looks key up in the level and loads its record into out (out->valid = 0 when there is none). */
void CScene__ConstructMap(void *level, CSceneMapRecord *out, s32 key) {
    s32 id;
    s32 **h1;
    s32 **h2;
    s32 size;
    s32 res;

    id = CScene__FindObjectType(level, key);
    if (id == -1) {
        out->valid = 0;
        return;
    }
    res = CScene__GetObject(level, id);
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
