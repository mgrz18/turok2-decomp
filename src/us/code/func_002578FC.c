#include "common.h"
#include "structs/player.h"
#include "globals/menu.h"
#include "globals/scene.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

s32 Level_GetHub(s32);

void CPlayer__ShowLevelNotCompleteScreen(s32 arg0, CPlayer *arg1)
{
  int new_var;
  u32 temp_a0;
  if (arg1->m_bLevelNotCompleteExit != 0)
  {
    temp_a0 = Level_GetHub(gScene_m_LevelID) - (new_var = 1);
    if ((temp_a0 < 6U) && (arg1->m_Data.m_dwLevelFirstTimeFlags & (new_var << temp_a0)))
    {
      MenuPool_OpenMenu((&gEngineApp_m_Game_m_MenuPool), (&gMissionFailedMenuDesc), (sptr) arg1, arg1->m_ContState.m_pContState, 0);
    }
    arg1->m_bLevelNotCompleteExit = 0;
  }
}
