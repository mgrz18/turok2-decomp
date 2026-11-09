/* Second-module copy of BlindOne_UpdateFight (src/us/code/BlindOne_UpdateFight.c), with its string/data addresses 0xE0 lower. */

#include "common.h"
#include "globals/onscreen.h"
s32 Animator_Update(s32);
s32 BlindOne_UpdateMover_2(s32);
s32 BlindOne_GetHealthPercent_2(s32);
s32 COnScreen__SetBossPercent(s32 *, s32);
extern s32 sBlindOnePhases_update_2; /* local type: this file typedefs its own s8...f64 instead of common.h (globals/boss.h has the alias) */
void BlindOne_UpdateFight_2(void *arg0)
{
  s32 (*temp_v0)();
  s32 var_s0;
  s32 var_s1;
  temp_v0 = *((typeof(&sBlindOnePhases_update_2)) (((s8 *) (&sBlindOnePhases_update_2)) + ((*((s32 *) (((s8 *) arg0) + 0x4FC))) * 8)));
  if (temp_v0 != 0)
  {
    temp_v0();
  }
  var_s0 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0x4EC))) > 0)
  {
    var_s1 = 0x3C;
    do
    {
      Animator_Update((sptr) (arg0 + var_s1));
      var_s0 += 1;
      var_s1 += 0x64;
      var_s0++;
      var_s0--;
    }
    while (var_s0 < (*((s32 *) (((s8 *) arg0) + 0x4EC))));
  }
  BlindOne_UpdateMover_2((sptr) (arg0 + 0x518));
  BlindOne_UpdateMover_2((sptr) (arg0 + 0x530));
  BlindOne_UpdateMover_2((sptr) (arg0 + 0x548));
  COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, BlindOne_GetHealthPercent_2((sptr) arg0));
}
