#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

s32 CPlayer__Warp(sptr, s32, s32, s32);


#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
/* host body (round 10 y2): the boss's m_AI.m_ModeTime (N64 + 0x180) and the player's
 * m_Data.m_bBlindBossComplete (N64 + 0x9E4) by member */
void BlindOne_PhaseVictoryUpdate(void **arg0)
{
  CGameObjectInstance *boss = (CGameObjectInstance *) *arg0;
  CPlayer *player;

  if (120.0f < boss->m_AI.m_ModeTime)
  {
    player = (CPlayer *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (player != NULL)
    {
      player->m_Data.m_bBlindBossComplete = 1;
      gEngineApp_m_Game.m_Settings.unlockedCheats |= 0x120;
      CPlayer__Warp((sptr) player, 0x1387, 2, 0);
    }
  }
}
#else
void BlindOne_PhaseVictoryUpdate(void **arg0)
{
  f32 new_var3;
  void *temp_v0;
  void *new_var2;
  GameSettings *temp_v1;
  s32 *new_var;
  new_var3 = *((f32 *) (((s8 *) (*arg0)) + 0x180));
  if (120.0f < new_var3)
  {
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    new_var2 = temp_v0;
    new_var = (s32 *) (((s8 *) new_var2) + 0x9E4);
    if (temp_v0 != 0)
    {
      temp_v1 = &gEngineApp_m_Game.m_Settings;
      *new_var = 1;
      temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 0x120);
      CPlayer__Warp((sptr) temp_v0, 0x1387, 2, 0);
    }
  }
}
#endif
