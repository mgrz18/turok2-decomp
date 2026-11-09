#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "protos/scene.h"


void CScene__GetTextureSet(CScene *arg0, s32 arg1, s32 *arg2, s32 arg3) {
    *arg2 = CISet__GetROMAddressAndSize(arg0->m_pTextureSets, arg0->m_TextureSetsKey, arg1, arg3);
}
