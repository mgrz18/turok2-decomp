#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
s32 CScene__GetObject(struct CScene *, s32);


s32 CScene__GetObject(CScene *arg0, s32 arg1) {
    s32 *table;

    table = arg0->m_pObjects;
    if (arg1 < *table) {
        return CISet__GetROMAddress((sptr)table, arg0->m_ObjectsKey, arg1);
    }
    return 0;
}
