#include "common.h"
#include "m2c_macros.h"

s32 func_00299EB0(s32, s32, s32, s32);
s32 func_0029B6F0(s32, s32, s32);
s32 func_0029B820(s32, s32, s32);
void func_0029E030(s32, s32);
void func_0029E340(s32, s32);

M2C_UNK func_002020C8(s32, void *);                 /* extern */

void func_00201FBC(s32 arg0)
{
  void *sp10;
  M2C_UNK sp14;
  s32 temp_a0;
  s32 var_a0;
  var_a0 = arg0 + 0x230;
  loop_1:
  func_0029B6F0(arg0 + 0x230, (s32) (&sp10), 1);

  func_0029E340(*((s32 *) (((s8 *) sp10) + 4)), *((s32 *) (((s8 *) sp10) + 8)));
  func_00299EB0(0, *((s32 *) (((s8 *) sp10) + 0)), *((s32 *) (((s8 *) sp10) + 4)), *((s32 *) (((s8 *) sp10) + 8)));
  func_0029B6F0(arg0 + 0xA48, (s32) (&sp14), 1);
  func_0029E030(*((s32 *) (((s8 *) sp10) + 4)), *((s32 *) (((s8 *) sp10) + 8)));
  temp_a0 = *((s32 *) (((s8 *) sp10) + 0xC));
  if (temp_a0 != 0)
  {
    func_0029B820(temp_a0, *((s32 *) (((s8 *) sp10) + 0x10)), 1);
  }
  func_002020C8(arg0, sp10);
  var_a0 = arg0 + 0x230;
  goto loop_1;
}
