#include "common.h"
#include "structs/game_object.h"

s32 AI_PlayAnim(s32, s32, s32);           /* extern */
extern s32 D_8012F5C0;

void MantidQueen_EnterMode4(CGameObjectInstance *arg0) {
    AI_PlayAnim(D_8012F5C0, D_8012F5C0 + 0x140, 0x5AB4);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}
