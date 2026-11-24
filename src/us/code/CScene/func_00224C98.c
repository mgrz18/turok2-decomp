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
void CScene__DecLinkGroupCount(struct CScene *, s32);


void CScene__DecLinkGroupCount(CScene *arg0, s32 arg1) {
    sptr temp_v0;
    u8 *temp_v0_2;

    temp_v0 = CISet__GetBlock(arg0->m_pBitSections, 1);
    CISet__GetBlock(temp_v0, 0);
    temp_v0_2 = CISet__GetBlock(temp_v0, 1) + arg1;
    *temp_v0_2 -= 1;
}
