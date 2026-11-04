#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);
s32 func_00246108(s32, s32, s32, s32, s32);

void func_00446C38(s32 arg0, CAI *arg1)
{
  u8 sp18[0x88];
  f32 new_var;
  func_00246108(arg0, (s32) arg1, (s32) (&sp18), 0, 1);
  new_var = arg1->m_ModeTime;
  if (arg1->m_ConditionTimer < new_var)
  {
    func_00243414(arg0, (s32) arg1, 5);
  }
}
