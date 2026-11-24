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
void CScene__ConstructWarp_2(struct CScene *);

void CScene__ConstructWarp_2(CScene *arg0) {
    arg0->m_WarpFade = 2;
    arg0->m_WarpState = 0;
    arg0->m_WarpTime = 0;
    arg0->m_bWarpSpawn = 0;
    arg0->m_LevelID = 0;
    arg0->m_WarpID = 0;
    arg0->unk_18FD8 = 0;
    arg0->m_PendingWarpID = 0;
}
