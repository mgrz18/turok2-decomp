#include "common.h"
#include "m2c_macros.h"

s32 func_0020B258(s32, s32, s32, s32, s32, s32, s32);
s32 func_0021D928(s32, s32, s32);

extern s32 D_800B6D1C;

void func_0025ECDC(s32 arg0, s32 arg1, s32 arg2)
{
  int new_var;
  func_0021D928(arg0, arg1, arg2);
  new_var = 0x110;
  func_0020B258(*((s32 *) (((s8 *) arg2) + 0xC)), *((s32 *) (((s8 *) arg0) + 0x98)), 1, arg0 + ((D_800B6D1C * 0x18) + new_var), 0, (s32) (*((s8 *) (((s8 *) arg2) + 0x1B))), -1);
}
