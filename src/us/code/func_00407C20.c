#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
s32 func_0027AD24(s32, s32, s32);

extern M2C_UNK D_004313B4;
extern M2C_UNK D_0043169C;
extern M2C_UNK D_800C046C;
extern M2C_UNK D_801100F0;
extern M2C_UNK D_8012FA24;

void func_00407C20(s32 arg0, int arg1)
{
  s32 *new_var3;
  M2C_UNK **new_var2;
  int new_var;
  func_0027AD24((s32) (&D_801100F0), arg0, (s32) (&D_800C046C));
  *((s32 *) (((s8 *) (&D_8012FA24)) + 0)) = -1;
  *((M2C_UNK **) (((s8 *) (&D_8012FA24)) + (-4))) = &D_0043169C;
  *((s8 *) (((s8 *) (&D_8012FA24)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + 4)) = -1;
  new_var3 = (s32 *) ((-0x14) + ((s8 *) (&D_8012FA24)));
  *new_var3 = -1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + (new_var = -0x10))) = -1;
  *(new_var2 = (M2C_UNK **) (((s8 *) (&D_8012FA24)) + (-0x18))) = &D_004313B4;
  func_00243414(arg0, arg1, 5);
}
