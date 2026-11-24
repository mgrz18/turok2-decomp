#include "common.h"
#include "structs/world.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__RequestWarp_2(struct CScene *, s32, s32);
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__RequestWarp_call CScene__RequestWarp_2
#else
void CScene__RequestWarp_call(struct CScene *, s32, s32) __asm__("CScene__RequestWarp_2");
#endif
void CScene__WarpToLevel_2(struct CScene *, s32);


void CScene__WarpToLevel_2(CScene *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        CScene__RequestWarp_call(arg0, ~arg1, 0);
    }
}
