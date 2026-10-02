#include "common.h"
#include "m2c_macros.h"

extern f32 D_800C051C;
extern f32 D_800C0520;
extern f32 D_800C0524;
extern f32 D_800C0528;
extern M2C_UNK D_8012F9DC;

void func_004075F8(void *arg0)
{
  f32 new_var;
  *((f32 *) (((s8 *) arg0) + 0x1FC)) = (f32) D_800C051C;
  if ((*((s32 *) (((s8 *) (&D_8012F9DC)) + 0))) == 3)
  {
    *((f32 *) (((s8 *) (&D_8012F9DC)) + 8)) = (f32) D_800C0520;
  }
  if ((*((s32 *) (((s8 *) (&D_8012F9DC)) + 0))) == 4)
  {
    *((s8 *) (((s8 *) (&D_8012F9DC)) + (-0xA))) = 1;
    *((f32 *) (((s8 *) (&D_8012F9DC)) + 8)) = (f32) D_800C0524;
  }
  new_var = (f32) D_800C0528;
  *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) arg0) + 0xD4))) | 0x2000);
  *((f32 *) (((s8 *) (&D_8012F9DC)) + 0xC)) = new_var;
}
