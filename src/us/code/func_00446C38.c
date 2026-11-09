#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

s32 AI_UpdateTargets(s32, s32, s32, s32, s32);

void BlindOne_BossMode3Update_2(s32 arg0, CAI *arg1)
{
  u8 sp18[0x88];
  f32 new_var;
  AI_UpdateTargets(arg0, (s32) arg1, (s32) (&sp18), 0, 1);
  new_var = arg1->m_ModeTime;
  if (arg1->m_ConditionTimer < new_var)
  {
    CAI__SetMode(arg0, (s32) arg1, 5);
  }
}
