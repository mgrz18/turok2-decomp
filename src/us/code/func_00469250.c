#include "common.h"
#include "m2c_macros.h"

void func_0026EDA8(s32);
s32 func_00285410(s32);

M2C_UNK func_0025359C(M2C_UNK *);                   /* extern */
extern M2C_UNK D_800F5A50;
extern M2C_UNK D_80119870;
extern M2C_UNK D_8011ACB0;

void func_00469250(void *arg0)
{
  s8 *new_var;
  func_00285410(0);
  func_0026EDA8((s32) (&D_800F5A50));
  func_0025359C(&D_80119870);
  new_var = (s8 *) (&D_8011ACB0);
  *((s32 *) (new_var + 0x24)) = 0;
  *((s32 *) (((s8 *) (&D_8011ACB0)) + 0x34)) = 0;
  *((s32 *) (((s8 *) (&D_8011ACB0)) + 0x44)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x23FE1)) = 2;
  *((s32 *) (((s8 *) arg0) + 0x23FDC)) = 1;
}
