#include "common.h"
#include "structs/boss.h"
#include "globals/onscreen.h"
#include "globals/boss.h"

s32 Animator_Update(sptr);
s32 BlindOne_UpdateMover(sptr);
s32 BlindOne_GetHealthPercent(sptr);
s32 COnScreen__SetBossPercent(s32 *, s32);

void BlindOne_UpdateFight(BlindOneState *arg0)
{
  void (*temp_v0)(void);
  s32 var_s0;
  s32 var_s1;
  temp_v0 = (void (*)(void)) sBlindOnePhases[arg0->phase].update;
  if (temp_v0 != 0)
  {
#ifdef TARGET_PORT
    /* ABI (docs/port/ABI.md section 5): MIPS passes arg0 through to the phase update (a0) */
    ((void (*)(BlindOneState *))temp_v0)(arg0);
#else
    temp_v0();
#endif
  }
  var_s0 = 0;
  if (arg0->faderCount > 0)
  {
    var_s1 = 0x3C;      /* faders[i] */
    do
    {
      Animator_Update((sptr) ((u8 *) arg0 + var_s1));
      var_s0 += 1;
      var_s1 += sizeof(ScriptAnimator);
      var_s0++;
      var_s0--;
    }
    while (var_s0 < arg0->faderCount);
  }
  BlindOne_UpdateMover((sptr) &arg0->movers[0]);
  BlindOne_UpdateMover((sptr) &arg0->movers[1]);
  BlindOne_UpdateMover((sptr) &arg0->movers[2]);
  COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, BlindOne_GetHealthPercent((sptr) arg0));
}
