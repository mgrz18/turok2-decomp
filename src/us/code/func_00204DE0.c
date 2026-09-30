#include "common.h"
#include "m2c_macros.h"

s32 func_00203330(s32, s32);
s32 func_0029B6F0(s32, s32, s32);
s32 func_0029B820(s32, s32, s32);
s32 func_0029DFF0(void);
void func_0029E010(s32);

extern M2C_UNK D_800D8DB0;
extern s32 D_800D8DCC;
extern M2C_UNK D_800D8DEC;

void func_00204DE0(void)
{
  s32 temp_a0;
  s8 *new_var;
  s32 temp_v0;
  s32 temp_v1;
  s32 temp_v1_2;
  s32 var_s0;
  if ((*((s32 *) (((s8 *) (&D_800D8DEC)) + 0))) != 0)
  {
    new_var = (s8 *) (&D_800D8DEC);
    *((s32 *) (((s8 *) (&D_800D8DEC)) + 0)) = (s32) ((*((s32 *) (((s8 *) (&D_800D8DEC)) + 0))) - 1);
  }
  *((s32 *) (((s8 *) (&D_800D8DEC)) + 4)) = (s32) ((*((s32 *) (((s8 *) (&D_800D8DEC)) + 4))) + 1);
  temp_a0 = func_0029DFF0();
  temp_v1 = D_800D8DCC + 1;
  D_800D8DCC = temp_v1;
  if (temp_v1 != 1)
  {
    func_0029E010(temp_a0);
    func_0029B6F0((s32) ((typeof(&D_800D8DEC)) (((s8 *) (&D_800D8DEC)) - 0x3C)), 0, 1);
    var_s0 = 0;
  }
  else
  {
    func_0029E010(temp_a0);
    var_s0 = 0;
  }
  do
  {
    func_00203330(0, 0);
    var_s0 += 1;
  }
  while (var_s0 < 0xA);
  temp_v0 = func_0029DFF0();
  temp_v1_2 = D_800D8DCC - 1;
  D_800D8DCC = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    func_0029E010(temp_v0);
    func_0029B820((s32) (&D_800D8DB0), 0, 1);
    return;
  }
  func_0029E010(temp_v0);
}
