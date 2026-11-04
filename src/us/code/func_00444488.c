#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);
s32 func_00409A64(s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x67C];
    /* 0x67C */ s32 counter;
} State;

extern State D_8012FB70;

void func_00444488(s32 arg0, CAI *ai)
{
  State *new_var;
  s32 temp_v0;
  if (ai->m_AnimRepeatedCount != 0)
  {
    new_var = &D_8012FB70;
    if (D_8012FB70.counter == ((D_8012FB70.counter / 3) * 3))
    {
      func_00409A64(arg0, new_var->counter == 0);
    }
    temp_v0 = D_8012FB70.counter + 1;
    D_8012FB70.counter = temp_v0;
    if (temp_v0 >= 0x1F)
    {
      func_00243414(arg0, (s32) ai, 0x11);
    }
  }
}
