#include "common.h"
#include "structs/boss.h"
#include "globals/onscreen.h"
#include "globals/boss.h"

void Animator_Update(void *slot);
void BlindOne_UpdateMover_2(BlindOneMover *mover);
s32 BlindOne_GetHealthPercent_2(BlindOneState *state);
void COnScreen__SetBossPercent(void *hud, s32 value);

void BlindOne_UpdateFight_2(BlindOneState *arg0)
{
  void (*temp_v0)(void);
  s32 var_s0;
  s32 var_s1;
  temp_v0 = (void (*)(void)) sBlindOnePhases_2[arg0->phase].update;
  if (temp_v0 != 0)
  {
    /* ABI (docs/port/ABI.md section 5): MIPS passes arg0 through to the phase update (a0) */
    ((void (*)(BlindOneState *))temp_v0)(arg0);
  }
  var_s0 = 0;
  if (arg0->faderCount > 0)
  {
    var_s1 = 0x3C;      /* faders[i] */
    do
    {
      Animator_Update((void *) ((u8 *) arg0 + var_s1));
      var_s0 += 1;
      var_s1 += sizeof(ScriptAnimator);
      var_s0++;
      var_s0--;
    }
    while (var_s0 < arg0->faderCount);
  }
  BlindOne_UpdateMover_2(&arg0->movers[0]);
  BlindOne_UpdateMover_2(&arg0->movers[1]);
  BlindOne_UpdateMover_2(&arg0->movers[2]);
  COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, BlindOne_GetHealthPercent_2(arg0));
}
