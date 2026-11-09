#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"


s32 DeathmatchMenu_OpenTeamBlood(s32 arg0, s32 arg1, s32 arg2)
{
  GameMatch *match;
  match = &gEngineApp_m_Settings.match;
  match->teamScore[0] = 0;
  match->teamScore[1] = 0;
  match->teamOver = 0;
  MenuPool_OpenChildMenu(arg2, arg1, &gTeamBloodMenuDesc);
  return 1;
}
