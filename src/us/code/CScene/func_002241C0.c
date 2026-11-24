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
u16 CScene__GetObjectTypeID(struct CScene *, s32);

u16 CScene__GetObjectTypeID(CScene *arg0, s32 arg1) {
    return arg0->m_pObjectTypes->ids[arg1];
}
