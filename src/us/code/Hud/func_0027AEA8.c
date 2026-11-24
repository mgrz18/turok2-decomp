#include "common.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/cache.h"
#include "protos/scene.h"

#include "structs/hud.h"

void Hud_GetImageFrame(FrameTable **ptable, s32 frame, void **pdata, u8 *pbyte);

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
      Hud_GetImageFrame((FrameTable **) temp_v0, arg1, &sp10, (u8 *) (&sp14));
      new_var = sp10;
      *arg2 = (s32) (*((u16 *) (((s8 *) new_var) + 4)));
      *arg3 = (s32) (*((u16 *) (((s8 *) new_var) + 6)));
      CCache__Unlock(0, temp_v0);
    }
  }
}
