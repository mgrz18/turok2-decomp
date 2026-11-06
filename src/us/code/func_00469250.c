#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

void RumblePool_FreeAll(s32);
s32 Controller_SetRumbleOptionAll(s32);
s32 func_0025359C(CPlayerPool *);                   /* extern */

/* The multiplayer match state at gEngineApp_m_Settings_match: the fields this resets. */

/* The game/menu context this runs on. */

extern s32 gRumblePool;

void func_00469250(CEngineApp *arg0)
{
  GameMatch *new_var;
  Controller_SetRumbleOptionAll(0);
  RumblePool_FreeAll((s32) (&gRumblePool));
  func_0025359C(&gEngineApp_m_Game.m_PlayerList);
  new_var = &gEngineApp_m_Settings.match;
  new_var->teams = 0;
  gEngineApp_m_Settings.match.tag = 0;
  gEngineApp_m_Settings.match.unk44 = 0;
  arg0->m_Settings.fadeMode = 2;
  arg0->m_Settings.nextMode = 1;
}
