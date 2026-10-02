#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
f32 func_00246310(s32, s32, s32, s32);

M2C_UNK func_00236314(void *, M2C_UNK *);           /* extern */
void *func_002532A8(M2C_UNK *);                     /* extern */
M2C_UNK func_002666B0(s32, s32, M2C_UNK);           /* extern */
extern M2C_UNK D_00430954;
extern M2C_UNK D_80119870;
extern u8 D_8012F3A0[];

void func_00402E6C(s32 arg0, void *arg1)
{
  M2C_UNK *var_s1;
  s32 var_s0;
  void *temp_v0;
  void *var_s2;
  s32 temp_v1;
  int new_var;
  var_s0 = arg0;
  var_s2 = arg1;
  temp_v0 = func_002532A8(&D_80119870);
  var_s1 = &D_8012F3A0;
  var_s1 = &D_8012F3A0;
  if (temp_v0 != 0)
  {
    new_var = 0xC;
    *((f32 *) (((s8 *) var_s0) + 0x50)) = (f32) ((*((f32 *) (((s8 *) var_s0) + 0x50))) + func_00246310(var_s0, *((s32 *) (((s8 *) temp_v0) + 4)), *((s32 *) (((s8 *) temp_v0) + 8)), *((s32 *) (((s8 *) temp_v0) + new_var))));
  }
  *((s32 *) (((s8 *) var_s0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xD4))) | 0x2000);
  if ((*((s8 *) (((s8 *) var_s2) + 0xC7))) != 0)
  {
    func_002666B0(*((s32 *) (((s8 *) (&D_8012F3A0)) + 0x220)), (*((s32 *) (((s8 *) (&D_8012F3A0)) + 0x220))) + 0x140, 0x5ADC);
    func_00243414(var_s0, (s32) var_s2, 7);
    temp_v1 = *((s32 *) (((s8 *) (*((void **) (((s8 *) (&D_8012F3A0)) + 0x214)))) + 0x14));
    if (temp_v1 != (-1))
    {
      func_00236314((temp_v1 * 0x68) + ((u8 *) (D_8012F3A0 + new_var)), &D_00430954);
    }
  }
}
