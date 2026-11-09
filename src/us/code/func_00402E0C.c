#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

extern s32 gMantidQueen_boss; /* local type: the field's real type (or the member path) changes the ROM's code here */

void MantidQueen_EnterMode5(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags = (s32) (*flags & ~0x2000);
    AI_PlayAnim(gMantidQueen_boss, gMantidQueen_boss + 0x140, 0x5ABE);    /* obj, &obj->m_AI */
}
