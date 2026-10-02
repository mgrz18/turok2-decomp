#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_80130970;
extern u8 D_80130974[];

void func_004214C4(void *arg0)
{
  s32 var_a0;
  u8 temp_v0;
  void *temp_a1;
  void *var_v1;
  temp_a1 = *((void **) (((s8 *) arg0) + 0x518));
  *((u8 *) (((s8 *) temp_a1) + 0x30)) = (u8) (*((u8 *) (((s8 *) (&D_80130970)) + 0)));
  *((u16 *) (((s8 *) temp_a1) + 0)) = (u16) (*((u16 *) (((s8 *) (&D_80130970)) + (-8))));
  *((u16 *) (((s8 *) temp_a1) + 2)) = (u16) (*((u16 *) (((s8 *) (&D_80130970)) + (-6))));
  *((u16 *) (((s8 *) temp_a1) + 4)) = (u16) (*((u16 *) (((s8 *) (&D_80130970)) + (-4))));
  var_a0 = 0;
  *((u16 *) (((s8 *) temp_a1) + 6)) = (u16) (*((u16 *) (((s8 *) (&D_80130970)) + (-2))));
  var_v1 = temp_a1;
  do
  {
    temp_v0 = *((u8 *) (D_80130974 + var_a0));
    var_v1 = temp_a1 + var_a0;
    var_a0 += 1;
    *((u8 *) (((s8 *) var_v1) + 0x33)) = temp_v0;
  }
  while (var_a0 < 8);
}
