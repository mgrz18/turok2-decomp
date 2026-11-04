#include "common.h"
#include "structs/game.h"

s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(s32 *);                     /* extern */
extern f32 D_800C093C;

void func_0040EB68(void **arg0)
{
  f32 new_var3;
  void *temp_v0;
  void *new_var2;
  GameSettings *temp_v1;
  s32 *new_var;
  new_var3 = *((f32 *) (((s8 *) (*arg0)) + 0x180));
  if (D_800C093C < new_var3)
  {
    temp_v0 = func_002532A8(&D_80119870.m_PlayerList);
    new_var2 = temp_v0;
    new_var = (s32 *) (((s8 *) new_var2) + 0x9E4);
    if (temp_v0 != 0)
    {
      temp_v1 = &D_80119870.m_Settings;
      *new_var = 1;
      temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 0x120);
      func_00248BA8((s32) temp_v0, 0x1387, 2, 0);
    }
  }
}
