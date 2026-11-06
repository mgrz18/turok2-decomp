#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/scene.h"

s32 Cinema_RequestById(s32);
s32 CScene__RequestWarp(s32, s32, s32);

CPlayer *CPlayerPool__GetFirstActive(void *);                     /* extern */
void CEngineApp__StartWarp(s32, s32);                       /* extern */
extern s32 gFinalEnding;
extern s32 gEndingPrimagenDefeated;
extern s32 gScreenHeight;
extern s32 sCreditsLine;
extern s32 gCreditsDone;
extern f32 gCreditsScroll;

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
  temp_v0 = CPlayerPool__GetFirstActive(new_var - 0x1268);        /* the game state, gEngineApp_m_Game */
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
    CScene__RequestWarp((s32) &gScene, ~Cinema_RequestById(0x82), 0);
  }
  else
  {
    CEngineApp__StartWarp(arg0, 0x82);
  }
  gFinalEnding = 1;
  gEndingPrimagenDefeated = 0;
}
