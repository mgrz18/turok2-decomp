#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 Cinema_PathPlaying();                                /* extern */
extern s32 gPersistentDataLoaded;

s32 Automap_IsShown(void *arg0)
{
  s32 var_s0;
  s32 var_v0;
  var_s0 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 4))) != 0)
  {
    var_v0 = 0;
    if (gPersistentDataLoaded != 0)
    {
      var_v0 = 0;
      if (Cinema_PathPlaying() == 0)
      {
        var_v0 = gEngineApp_m_Settings.nextMode ^ 1;
        if (1)
        {
          var_s0 = ((u32) var_v0) > 0U;
          goto block_4;
        }
      }
    }
  }
  else
  {
    block_4:
    ;

  }
  return var_s0;
}
