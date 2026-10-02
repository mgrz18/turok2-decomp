#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00285CC4(void *, void *);              /* extern */
extern s32 D_801309C0;

void func_0045B684(void *arg0)
{
  void *temp_a0;
  void *temp_a1;
  s32 new_var;
  temp_a1 = *((void **) (((s8 *) arg0) + 0xC));
  *((s32 *) (((s8 *) temp_a1) + 0x120)) = (s32) ((*((s32 *) (((s8 *) temp_a1) + 0x120))) & 0xFBFFFFFF);
  temp_a0 = *((void **) (((s8 *) arg0) + 0xC));
  new_var = (s32) ((*((s32 *) (((s8 *) temp_a0) + 0x120))) | 8);
  D_801309C0 = 8;
  *((s32 *) (((s8 *) temp_a0) + 0x120)) = new_var;
  func_00285CC4(temp_a0, temp_a1);
}
