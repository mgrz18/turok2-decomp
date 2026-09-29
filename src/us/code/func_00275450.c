#include "common.h"
#include "m2c_macros.h"

s32 func_0028F518(s32, s32);
s32 func_00290E40(s32, s32, s32);

M2C_UNK func_00290E10(s32, s32);                    /* extern */
M2C_UNK func_00290E90(s32, s32);                    /* extern */
M2C_UNK func_00290EC0(s32, s16);                    /* extern */
extern M2C_UNK D_800A8200;

void func_00275450(void *arg0)
{
  f32 temp_f20;
  s32 temp_a1;
  s32 var_s0;
  var_s0 = 0;
  func_0028F518(*((s32 *) (((s8 *) arg0) + 0x10)), *((s32 *) (((s8 *) arg0) + 0xC)));
  func_00290E90(*((s32 *) (((s8 *) arg0) + 0x14)), *((s32 *) (((s8 *) arg0) + 0x10)));
  func_00290E10(*((s32 *) (((s8 *) arg0) + 0x14)), *((s32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 8)))) + 4)));
  do
  {
    temp_a1 = var_s0 & 0xFF;
    func_00290E40(*((s32 *) (((s8 *) arg0) + 0x14)), temp_a1, 0);
    var_s0 += 1;
  }
  while (var_s0 < 0x14);
  temp_f20 = ((f32) (*((s32 *) (((s8 *) arg0) + 0x24)))) * ((*((f32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0)))) + 0x2BA4))) * (*((f32 *) (((s8 *) (&D_800A8200)) + 4))));
  func_00290EC0(*((s32 *) (((s8 *) arg0) + 0x14)), (s16) ((s32) (temp_f20 * 32767.0f)));
  *((f32 *) (((s8 *) arg0) + 0x2C)) = temp_f20;
}
