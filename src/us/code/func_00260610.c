#include "common.h"
#include "structs/game_object.h"

/* AI callback: clears the AI's 0x6C-0x74 words and the actor's flag 0x20000. */
void func_00260610(CGameObjectInstance *obj, CAI *ai) {
    ai->m_vTargetOffset.x = 0;
    ai->m_vTargetOffset.y = 0;
    ai->m_vTargetOffset.z = 0;
    obj->m_dwFlags &= ~0x20000;
}
