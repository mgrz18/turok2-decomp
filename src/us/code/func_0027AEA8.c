#include "common.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/cache.h"
#include "protos/scene.h"

s32 Hud_GetImageFrame(s32, s32, s32, s32);

void Hud_GetSpriteSize(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3)
{
  void *sp10;
  s32 sp14;
  void *new_var;
  sptr temp_v0;
  if (gPersistentDataLoaded != 0)
  {
    temp_v0 = CScene__LoadGraphicSet((&gScene), arg0, 0, 1);
    if (temp_v0 != 0)
    {
      Hud_GetImageFrame(temp_v0, arg1, (sptr) (&sp10), (sptr) (&sp14));
      new_var = sp10;
      *arg2 = (s32) (*((u16 *) (((s8 *) new_var) + 4)));
      *arg3 = (s32) (*((u16 *) (((s8 *) new_var) + 6)));
      CCache__Unlock(0, temp_v0);
    }
  }
}
