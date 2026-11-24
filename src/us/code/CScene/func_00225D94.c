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
void *CScene__GetCurve(struct CScene *, s32);

/* Curve i: the table starts with the element size, the data at +8. */
void *CScene__GetCurve(CScene *arg0, s32 arg1) {
    s32 *t = arg0->m_pCurves;
    return (s8 *)t + (arg1 * *t + 8);
}
