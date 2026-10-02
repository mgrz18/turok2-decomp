#include "common.h"
#include "m2c_macros.h"

s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(M2C_UNK *);                     /* extern */
extern f32 D_800C093C;
extern M2C_UNK D_80119870;

void func_0040EB68(void **arg0)
{
  f32 new_var3;
  void *temp_v0;
  void *new_var2;
  void *temp_v1;
  s32 *new_var;
  new_var3 = *((f32 *) (((s8 *) (*arg0)) + 0x180));
  if (D_800C093C < new_var3)
  {
    temp_v0 = func_002532A8(&D_80119870);
    new_var2 = temp_v0;
    new_var = (s32 *) (((s8 *) new_var2) + 0x9E4);
    if (temp_v0 != 0)
    {
      temp_v1 = (typeof(&D_80119870)) (((s8 *) (&D_80119870)) + 0x1268);
      *new_var = 1;
      *((s32 *) (((s8 *) temp_v1) + 4)) = (s32) ((*((s32 *) (((s8 *) temp_v1) + 4))) | 0x120);
      func_00248BA8((s32) temp_v0, 0x1387, 2, 0);
    }
  }
}
