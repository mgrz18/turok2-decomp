#include "common.h"
#include "m2c_macros.h"

s32 func_00253DE0(s32, s32);

extern M2C_UNK D_800A72A0;
extern M2C_UNK D_800AF618;

void func_00258E38(void *arg0, void *arg1)
{
  s32 new_var;
  new_var = *((s32 *) (((s8 *) arg0) + 0x1A8));
  *((s32 *) (((s8 *) arg1) + 0x64)) = 0;
  func_00253DE0(new_var, 0x1A5);
  *((f32 *) (((s8 *) arg1) + 0x118)) = (f32) ((*((f32 *) (((s8 *) (*((typeof(&D_800AF618)) ((0, ((s8 *) (&D_800AF618)) + ((*((s16 *) (((s8 *) (*((s32 *) (((s8 *) arg0) + 0x1A8)))) + 0x996))) * 4)))))) + 0x18))) * (*((f32 *) (((s8 *) (&D_800A72A0)) + 4))));
}
