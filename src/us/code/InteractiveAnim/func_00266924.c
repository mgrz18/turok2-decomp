#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"



void InteractiveAnim_EventHide(CGameObjectInstance *arg0)
{
  int new_var;
  new_var = ~0x2000;
  arg0->m_dwFlags = (s32) ((arg0->m_dwFlags & new_var) & (~0x100));
  CScene__SetInstanceSavedBit((&gScene), arg0, 1);
}
