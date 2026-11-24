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
void CScene__GetTextureSet(struct CScene *, s32, s32 *, sptr);


void CScene__GetTextureSet(CScene *arg0, s32 arg1, s32 *arg2, sptr arg3) {
    *arg2 = CISet__GetROMAddressAndSize(arg0->m_pTextureSets, arg0->m_TextureSetsKey, arg1, (s32 *)arg3);
}
