#include "common.h"
#include "structs/game_object.h"

s32 Action_DrawFade(s32, s32);

void Action_ChildVanishEnter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = Action_DrawFade;
}
