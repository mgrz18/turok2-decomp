#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

extern s32 gMantidQueen_boss; /* local type: the field's real type (or the member path) changes the ROM's code here */

void MantidQueen_EnterMode3(CGameObjectInstance *arg0) {
    AI_PlayAnim(gMantidQueen_boss, gMantidQueen_boss + 0x140, 0x5AAA);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
