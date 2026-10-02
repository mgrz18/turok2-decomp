#include "common.h"
#include "m2c_macros.h"

s32 func_00219F74(s32, s32, s32);
s32 func_0027AD24(s32, s32, s32);
s32 func_004089E0(s32, s32, s32);

M2C_UNK func_0027AD54(M2C_UNK *, void *, M2C_UNK *); /* extern */
extern M2C_UNK D_800C0560;
extern M2C_UNK D_800C0568;
extern M2C_UNK D_801100F0;

void func_00441B20(void *arg0, void *arg1)
{
  void *temp_a3;
  void *temp_s0;
  int new_var;
  *((void **) (((s8 *) arg0) + 0)) = arg1;
  *((s32 *) (((s8 *) arg0) + 4)) = (s32) ((*((void **) (((s8 *) arg1) + 0x14))) + 0x14);
  new_var = (*((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x14)))) + 0x54))) * 0xF;
  *((s32 *) (((s8 *) arg0) + 8)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x10)) = 0;
  *((f32 *) (((s8 *) arg0) + 0xC)) = (f32) new_var;
  *((s32 (**)(s32, s32, s32)) (((s8 *) arg1) + 0x234)) = func_004089E0;
  temp_s0 = *((void **) (((s8 *) arg0) + 0));
  *((s32 *) (((s8 *) arg0) + 0x14)) = 0;
  temp_a3 = temp_s0 + 0x140;
  *((s8 *) (((s8 *) temp_a3) + 0xC6)) = func_00219F74((s32) temp_s0, 0x5334, -1);
  *((s16 *) (((s8 *) temp_a3) + 0xC4)) = 0x5334;
  *((s32 *) (((s8 *) temp_s0) + 0x140)) = (s32) ((*((s32 *) (((s8 *) temp_s0) + 0x140))) & (~1));
  *((s8 *) (((s8 *) temp_a3) + 0xC7)) = 0;
  func_0027AD24((s32) (&D_801100F0), (s32) arg1, (s32) (&D_800C0560));
  func_0027AD54(&D_801100F0, arg1, &D_800C0568);
}
