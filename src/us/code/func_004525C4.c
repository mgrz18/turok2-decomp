#include "common.h"
#include "m2c_macros.h"

void func_00285A80(s32);
s32 func_00288C5C(s32, s32);

extern M2C_UNK D_800F6CB0;
extern s32 D_801308F8;

s32 func_004525C4(s32 arg0, void *arg1)
{
  s8 *new_var2;
  int new_var;
  func_00288C5C((s32) (&D_800F6CB0), 8);
  new_var2 = (s8 *) (*((void **) (((s8 *) arg1) + 0x20)));
  D_801308F8 = 0;
  new_var = 1;
  func_00285A80((s32) (*((s8 *) (new_var2 + 4))));
  return new_var;
}
