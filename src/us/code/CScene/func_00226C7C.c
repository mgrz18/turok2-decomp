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
sptr CScene__GetLinkGroupCountPtr(struct CScene *, s32);


sptr CScene__GetLinkGroupCountPtr(CScene *arg0, s32 arg1) {
    sptr temp_v0;

    temp_v0 = CISet__GetBlock(arg0->m_pBitSections, 1);
    CISet__GetBlock(temp_v0, 0);
    return CISet__GetBlock(temp_v0, 1) + arg1;
}
