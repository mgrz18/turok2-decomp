#include "common.h"
#include "structs/game_object.h"

s32 CAI__Update(CGameObjectInstance *, CAI *);

/* Updates the actor's AI (CGameObjectInstance.m_AI, 0x140). */
void func_0021F1B4(CGameObjectInstance *arg0) {
    CAI__Update(arg0, &arg0->m_AI);
}
