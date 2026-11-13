#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"
#include "protos/scene.h"


#ifdef TARGET_PORT
/* The port (round 13 b2): the bit sections by member of CScene (the N64 offset 0x6C is another
 * member of the host CScene). */
#include "structs/world.h"
void CScene__SetEventFiredInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    CScene *w = arg0;
    u8 *p = (u8 *) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(w->m_pBitSections, 0), arg1), 2), 1) + arg2 / 8;
    if (arg3 != 0) {
        *p |= 1 << (arg2 & 7);
    } else {
        *p &= ~(1 << (arg2 & 7));
    }
}
#else
void CScene__SetEventFiredInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
  s32 temp_a0;
  sptr temp_a1;
  sptr temp_v0;
  sptr new_var;
  s32 var_a2;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*((s32 *) (((s8 *) arg0) + 0x6C)), 0), arg1), 2);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg2 & 7);
  if (arg3 != 0)
  {
    var_v1 = arg2;
    if (arg2 < 0)
    {
      var_v1 = arg2 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  var_a2 = arg2;
  if (var_a2 < 0)
  {
    var_a2 += 7;
  }
  temp_v0_2 = temp_a1 + (var_a2 >> 3);
  *temp_v0_2 &= ~temp_a0;
}
#endif
