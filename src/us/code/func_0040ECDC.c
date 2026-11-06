#include "common.h"
#include "structs/boss.h"
#include "globals/onscreen.h"


/* The update entries of the phase table: sBlindOnePhases_update is sBlindOnePhases + 4
 * (BlindOnePhase.update), which the ROM addresses by its own symbol. */
typedef struct {
    /* 0x0 */ void (*fn)(void);
    /* 0x4 */ s32 unk4;
} ModeHandler;

s32 Animator_Update(s32);
s32 BlindOne_UpdateMover(s32);
s32 BlindOne_GetHealthPercent(s32);
s32 COnScreen__SetBossPercent(s32 *, s32);
extern ModeHandler sBlindOnePhases_update[];

void BlindOne_UpdateFight(BlindOneState *arg0)
{
  void (*temp_v0)(void);
  s32 var_s0;
  s32 var_s1;
  temp_v0 = sBlindOnePhases_update[arg0->phase].fn;
  if (temp_v0 != 0)
  {
    temp_v0();
  }
  var_s0 = 0;
  if (arg0->faderCount > 0)
  {
    var_s1 = 0x3C;      /* faders[i] */
    do
    {
      Animator_Update((s32) ((u8 *) arg0 + var_s1));
      var_s0 += 1;
      var_s1 += sizeof(ScriptAnimator);
      var_s0++;
      var_s0--;
    }
    while (var_s0 < arg0->faderCount);
  }
  BlindOne_UpdateMover((s32) &arg0->movers[0]);
  BlindOne_UpdateMover((s32) &arg0->movers[1]);
  BlindOne_UpdateMover((s32) &arg0->movers[2]);
  COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, BlindOne_GetHealthPercent((s32) arg0));
}
