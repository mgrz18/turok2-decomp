#include "common.h"
#include "structs/game_object.h"

s32 CScene__MarkGeneratorsSaved(s32);

extern s32 gScene;

void MantidQueen_EnterMode2(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    CScene__MarkGeneratorsSaved((s32) &gScene);
}
