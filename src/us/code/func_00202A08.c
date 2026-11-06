#include "globals/cache.h"

extern s32 gHashTable;
void CHashTable__Remove(u32 arg0)
{
  s32 *temp_v0;
  s32 *var_a0;
  s32 *var_a2;
  s32 *var_v1;
  var_v1 = gHashTable + ((((((arg0 << 5) ^ (arg0 >> 1)) ^ (arg0 >> 9)) ^ (arg0 >> 0x11)) & gHashMask) * 0x10);
  var_a2 = 0;
  if ((*var_v1) != arg0)
  {
    loop_1:
    var_a2 = var_v1;

    var_v1 = *((s32 **) (((s8 *) var_v1) + 0xC));
    if (var_v1 != 0)
    {
 do { if ((*var_v1) == arg0) { goto block_3; } } while (0);
      goto loop_1;
    }
  }
  else
  {
    block_3:
    if (var_v1 != 0)
    {
      var_a0 = *((s32 **) (((s8 *) var_v1) + 0xC));
      if (var_a0 != 0)
      {
        *((s32 *) (((s8 *) var_v1) + 4)) = (s32) (*((s32 *) (((s8 *) var_a0) + 4)));
        *((s32 *) (((s8 *) var_v1) + 0)) = *((s32 *) (((s8 *) var_a0) + 0));
        *((s32 **) (((s8 *) var_v1) + 0xC)) = (s32 *) (*((s32 **) (((s8 *) var_a0) + 0xC)));
        temp_v0 = var_v1;
        var_v1 = var_a0;
        var_a0 = temp_v0;
      }
      if (var_a2 != 0)
      {
        *((s32 **) (((s8 *) var_a2) + 0xC)) = var_a0;
      }
      *((s32 *) (((s8 *) var_v1) + 4)) = 0;
      *((s32 *) (((s8 *) var_v1) + 0)) = 0;
      *((s32 *) (((s8 *) var_v1) + 0xC)) = 0;
    }

  }
}
