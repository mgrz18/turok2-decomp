#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"
#include "structs/event.h"

s32 CGameObjectInstance__AdvanceAnimation(CGameObjectInstance *);

/* The mode parameter this AI callback is handed: only the field it reads. */

void InteractiveAnim_EventPlay(CGameObjectInstance *arg0, sptr arg1, LevelEvent *arg2) {
    AI_PlayAnim(arg0, arg1, (s32) arg2->param);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    CGameObjectInstance__AdvanceAnimation((CGameObjectInstance *) arg0);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}
