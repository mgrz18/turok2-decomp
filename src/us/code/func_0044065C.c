#include "common.h"
#include "structs/game_object.h"

extern f32 D_800C052C;
extern f32 D_800C0530;
extern s32 D_8012F9E8;          /* f32, with another f32 just before it */

void func_0044065C(CGameObjectInstance *arg0)
{
  unsigned int new_var3;
  f32 new_var4;
  s32 *new_var;
  f32 new_var2;
  new_var3 = arg0->m_dwFlags | 0x2000;
  new_var4 = D_800C052C;
  new_var = &arg0->m_dwFlags;
  new_var2 = (f32) D_800C0530;
  *new_var = (s32) new_var3;
  ((f32 *) &D_8012F9E8)[0] = (f32) new_var4;
  ((f32 *) &D_8012F9E8)[-1] = new_var2;
}
