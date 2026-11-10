
#include "protos/ai.h"
s32 AI_UpdateTargets(s32, s32, s32, s32, s32);
void BlindOne_BossMode3Update(sptr arg0, sptr arg1)
{
  u8 sp18[0x88];
  AI_UpdateTargets(arg0, arg1, (sptr) (&sp18), 0, 1);
  if ((*((f32 *) (((s8 *) arg1) + 0x40))) > (*((f32 *) (((s8 *) arg1) + 0x64))))
  {
    CAI__SetMode(arg0, arg1, 5);
  }
}
