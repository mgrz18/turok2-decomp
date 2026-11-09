#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"



void MantidQueen_EnterMode2(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    CScene__MarkGeneratorsSaved(&gScene);
}
