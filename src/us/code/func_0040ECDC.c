#include "common.h"
#include "structs/boss.h"

/* The Blind One's per-frame update (D_80130200, see structs/boss.h): runs the
 * phase handler, steps the animators, the three movers and the health bar. */

/* Per-mode handlers, 8 bytes each. */
typedef struct {
    /* 0x0 */ void (*fn)(void);
    /* 0x4 */ s32 unk4;
} ModeHandler;

s32 func_00235510(s32);
s32 func_0040C850(s32);
s32 func_0040CB28(s32);
s32 func_0027AD88(s32 *, s32);
extern ModeHandler D_00432410[];
extern s32 D_801100F0;

void func_0040ECDC(BlindOneState *arg0)
{
  void (*temp_v0)(void);
  s32 var_s0;
  s32 var_s1;
  temp_v0 = D_00432410[arg0->phase].fn;
  if (temp_v0 != 0)
  {
    temp_v0();
  }
  var_s0 = 0;
  if (arg0->slotCount > 0)
  {
    var_s1 = 0x3C;      /* &arg0->slots[i] */
    do
    {
      func_00235510((s32) ((u8 *) arg0 + var_s1));
      var_s0 += 1;
      var_s1 += sizeof(ScriptAnimator);
      var_s0++;
      var_s0--;
    }
    while (var_s0 < arg0->slotCount);
  }
  func_0040C850((s32) &arg0->movers[0]);
  func_0040C850((s32) &arg0->movers[1]);
  func_0040C850((s32) &arg0->movers[2]);
  func_0027AD88(&D_801100F0, func_0040CB28((s32) arg0));
}
