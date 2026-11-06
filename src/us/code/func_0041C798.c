#include "common.h"
#include "m2c_macros.h"

s32 Video_GetResolutionText(void);

s32 GetMemSize();                                /* extern */
extern f32 gFrameIncrement;
extern f32 sResolutionToggleDelay;

s32 OptionsMenu_ResolutionText(void *arg0)
{
  f32 temp_f0;
  s32 var_v0;
  int new_var2;
  s8 *new_var;
  temp_f0 = sResolutionToggleDelay - gFrameIncrement;
  sResolutionToggleDelay = temp_f0;
  if (temp_f0 <= 0.0f)
  {
    sResolutionToggleDelay = 0.0f;
  }
  if (GetMemSize() != 0x400000)
  {
    new_var2 = 0x01000000;
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) | new_var2;
  }
  else
  {
    var_v0 = (*((s32 *) (((s8 *) arg0) - -8))) & 0xFEFFFFFF;
  }
  new_var2 = var_v0;
  *((s32 *) (((s8 *) arg0) + 8)) = new_var2;
  new_var = ((s8 *) arg0) + 0x14;
  *((s32 *) new_var) = Video_GetResolutionText();
  return 0;
}
