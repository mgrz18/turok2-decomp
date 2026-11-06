#include "common.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

extern s32 gEngineApp_m_Settings_match;

s32 DeathmatchMenu_OpenTeamBlood_2(s32 arg0, s32 arg1, s32 arg2)
{
  s8 *new_var;
  new_var = (s8 *) (&gEngineApp_m_Settings_match);
  *((s32 *) (new_var + 0x2C)) = 0;
  *((s32 *) (new_var + 0x30)) = 0;
  *((s32 *) (new_var + 0x28)) = 0;
  MenuPool_OpenChildMenu_2(arg2, arg1, (s32) (&gTeamBloodMenuDesc_2));
  return 1;
}
