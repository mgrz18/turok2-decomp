#include "common.h"
#include "structs/game_object.h"

s32 AI_PlayAnim(s32, s32, s32);           /* extern */
extern s32 gMantidQueen_boss;

void MantidQueen_EnterMode6(CGameObjectInstance *arg0) {
    AI_PlayAnim(gMantidQueen_boss, gMantidQueen_boss + 0x140, 0x5AB4);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}
