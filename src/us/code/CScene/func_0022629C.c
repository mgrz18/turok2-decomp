#include "common.h"
#include "protos/cache.h"
/* The request CCache__LoadPersistantSubBlock leaves in v0 (protos/cache.h declares the N64 definition
 * void): the N64 symbol by another C name, returning it; the port's definition returns it. */
#ifdef TARGET_PORT
#define CCache__LoadPersistantSubBlock_v(...) ((void **) CCache__LoadPersistantSubBlock(__VA_ARGS__))
#else
void **CCache__LoadPersistantSubBlock_v() __asm__("CCache__LoadPersistantSubBlock");
#endif
#include "globals/strings.h"
#include "m2c_macros.h"
#include "structs/world.h"
#include "structs/cache.h"
#include "protos/iset.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__GetLevelName(struct CScene *, s32, void *, s32);


void *func_0029E460(void *, void *, s32);           /* memcpy */

/* Copies the level's name (sub-block 8 of its level file, a handle: data then size) into
 * arg2, at most arg3 bytes. The level file stays loaded when it is the current level. */
void CScene__GetLevelName(struct CScene *arg0, s32 arg1, void *arg2, s32 arg3) {
    sptr sp20;
    sptr temp_s2;
    s32 temp_v0;
    s32 var_a2;
    CMemEntry *temp_v0_2;

    temp_v0 = CISet__GetROMAddress((sptr) arg0->m_pLevels, arg0->m_LevelsKey, arg1);
    temp_s2 = CCache__LoadPersistantIndex(0, (sptr) &sp20, temp_v0, (sptr) &sCSceneCacheDesc_LevelIndex);
    temp_v0_2 = (CMemEntry *) CCache__LoadPersistantSubBlock_v(0, sp20, 8, temp_v0, (sptr) arg0, 0, (sptr) &sCSceneCacheDesc_LevelName);
    var_a2 = temp_v0_2->size;
    if (arg3 < var_a2) {
        var_a2 = arg3;
    }
    func_0029E460(arg2, temp_v0_2->data, var_a2);
    CCache__Deallocate(0, (sptr) temp_v0_2);
    if (arg0->m_nLevel != arg1) {
        CCache__Deallocate(0, temp_s2);
    }
}
