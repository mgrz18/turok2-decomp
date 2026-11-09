#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "protos/scene.h"
#include "protos/player.h"

#ifdef TARGET_PORT /* host layout: CGame.m_Settings (structs/game.h) */
#include "structs/game.h"
#define GAME_SETTINGS_OFF T2_OFFSETOF(CGame, m_Settings)
#else
#define GAME_SETTINGS_OFF 0x1268
#endif

s32 Cinema_RequestById(s32);

void CEngineApp__StartWarp(s32, s32);                       /* extern */

void EngineMode16_Ending_Init(s32 arg0)
{
  CPlayer *temp_v0;
  s8 *new_var;
  new_var = (s8 *) (&gEngineApp_m_Settings);
  gEngineApp_m_Settings.multiplayer = 0;
  gFinalEnding = 0;
  sCreditsLine = 0;
  gCreditsScroll = (f32) gScreenHeight;
  gCreditsDone = 0;
  temp_v0 = CPlayerPool__GetFirstActive(new_var - GAME_SETTINGS_OFF);        /* the game state, gEngineApp_m_Game */
  if ((temp_v0 != 0) && (temp_v0->m_Data.m_bPrimagenBossComplete != 0))
  {
    gEndingPrimagenDefeated = 1;
  }
  else
  {
    gEndingPrimagenDefeated = 0;
  }
  if (gEndingPrimagenDefeated != 0)
  {
    CScene__RequestWarp(&gScene, ~Cinema_RequestById(0x82), 0);
  }
  else
  {
    CEngineApp__StartWarp(arg0, 0x82);
  }
  gFinalEnding = 1;
  gEndingPrimagenDefeated = 0;
}
