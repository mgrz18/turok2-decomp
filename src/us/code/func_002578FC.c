#include "common.h"
#include "structs/player.h"
#include "globals/menu_desc.h"

s32 Level_GetHub(s32);
s32 MenuPool_OpenMenu(s32, s32, s32, s32, s32);
extern s32 gScene_m_LevelID;
extern s32 gEngineApp_m_Game_m_MenuPool;

void CPlayer__ShowLevelNotCompleteScreen(s32 arg0, CPlayer *arg1)
{
  int new_var;
  u32 temp_a0;
  if (arg1->m_bLevelNotCompleteExit != 0)
  {
    temp_a0 = Level_GetHub(gScene_m_LevelID) - (new_var = 1);
    if ((temp_a0 < 6U) && (arg1->m_Data.m_dwLevelFirstTimeFlags & (new_var << temp_a0)))
    {
      MenuPool_OpenMenu((s32) (&gEngineApp_m_Game_m_MenuPool), (s32) (&gMissionFailedMenuDesc), (s32) arg1, arg1->m_ContState.m_pContState, 0);
    }
    arg1->m_bLevelNotCompleteExit = 0;
  }
}
