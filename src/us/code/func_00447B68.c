#include "common.h"
#include "structs/game.h"

s32 CPlayer__Warp(s32, s32, s32, s32);

void *CPlayerPool__GetFirstActive(s32 *);                     /* extern */
extern f32 D_800C093C;

void BlindOne_PhaseVictoryUpdate_2(void **arg0)
{
  f32 new_var;
  void *temp_v0;
  GameSettings *temp_v1;
  new_var = *((f32 *) (((s8 *) (*arg0)) + 0x180));
  if (D_800C093C < new_var)
  {
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (temp_v0 != 0)
    {
      temp_v1 = &gEngineApp_m_Game.m_Settings;
      *((s32 *) (((s8 *) temp_v0) + 0x9E4)) = 1;
      temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 0x120);
      CPlayer__Warp((s32) temp_v0, 0x1387, 2, 0);
    }
  }
}
