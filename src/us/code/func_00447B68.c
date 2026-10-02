#include "common.h"
#include "m2c_macros.h"

s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(M2C_UNK *);                     /* extern */
extern f32 D_800C093C;
extern M2C_UNK D_80119870;

void func_00447B68(void **arg0)
{
  f32 new_var;
  void *temp_v0;
  void *temp_v1;
  new_var = *((f32 *) (((s8 *) (*arg0)) + 0x180));
  if (D_800C093C < new_var)
  {
    temp_v0 = func_002532A8(&D_80119870);
    if (temp_v0 != 0)
    {
      temp_v1 = (typeof(&D_80119870)) (((s8 *) (&D_80119870)) + 0x1268);
      *((s32 *) (((s8 *) temp_v0) + 0x9E4)) = 1;
      *((s32 *) (((s8 *) temp_v1) + 4)) = (s32) ((*((s32 *) (((s8 *) temp_v1) + 4))) | 0x120);
      func_00248BA8((s32) temp_v0, 0x1387, 2, 0);
    }
  }
}
