#include "common.h"
#include "m2c_macros.h"

s32 func_002031E8(s32, s32);
s32 func_002062F8(s32, s32);
s32 func_002063B4(s32, s32, s32, s32);
s32 func_00206498(s32, s32);
s32 func_0029B6F0(s32, s32, s32);
s32 func_0029B820(s32, s32, s32);
s32 func_0029DFF0(void);
void func_0029E010(s32);

extern M2C_UNK D_800D8DB0;
extern s32 D_800D8DCC;

s32 func_00205EFC(s32 arg0, s32 arg1, s32 arg2)
{
  s32 temp_a0;
  s32 temp_v0;
  s32 temp_v0_2;
  s32 temp_v0_3;
  s32 temp_v1;
  s32 temp_v1_2;
  s32 var_s0;
  temp_a0 = func_0029DFF0();
  temp_v1 = D_800D8DCC + 1;
  D_800D8DCC = temp_v1;
  if (temp_v1 != 1)
  {
    func_0029E010(temp_a0);
    func_0029B6F0((s32) (&D_800D8DB0), 0, 1);
  }
  else
  {
    func_0029E010(temp_a0);
  }
  var_s0 = func_002031E8(0, 0x22);
  if (var_s0 != 0)
  {
    *((s32 *) (((s8 *) var_s0) + 8)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 8))) + 1);
    *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x100);
    temp_v0 = func_002063B4(0, arg2, 1, 0x22);
    *((s32 *) (((s8 *) var_s0) + 0)) = temp_v0;
    if (temp_v0 != 0)
    {
      *((s32 *) (((s8 *) var_s0) + 4)) = arg2;
      *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x22);
      func_00206498(0, var_s0);
    }
    else
    {
      temp_v0_2 = (*((s32 *) (((s8 *) var_s0) + 8))) - 1;
      *((s32 *) (((s8 *) var_s0) + 8)) = temp_v0_2;
      if (temp_v0_2 == 0)
      {
        *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) & (~0x100));
      }
      func_002062F8(0, var_s0);
      var_s0 = 0;
    }
  }
  temp_v0_3 = func_0029DFF0();
  temp_v1_2 = D_800D8DCC - 1;
  D_800D8DCC = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    if (temp_v1_2 || temp_v0_2)
    {
      func_0029E010(temp_v0_3);
      func_0029B820((s32) (&D_800D8DB0), 0, 1);
      return var_s0;
    }
    else
    {
      func_0029E010(temp_v0_3);
      func_0029B820((s32) (&D_800D8DB0), 0, 1);
      return var_s0;
    }
  }
  func_0029E010(temp_v0_3);
  return var_s0;
}
