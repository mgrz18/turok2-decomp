#ifdef TARGET_PORT
/* The port: the instance's definition by member (CInstanceHeader.m_pIntelligence, 0x14 on the
 * N64, 0x18 on the host); the enemy definition's words are at the same offsets. */
#include "common.h"
#include "structs/game_object.h"
s32 func_00268120(void *arg0) {
    CIntelligence *def = ((CGameObjectInstance *) arg0)->header.m_pIntelligence;

    if (def->m_Type != 1) {
        return 0;
    }
    return *(s32 *) ((u8 *) def + 0x48) & 0x4000;
}
#else

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
s32 func_00268120(void *arg0)
{
  void *new_var;
  void *temp_a0;
  temp_a0 = *((void **) (((s8 *) arg0) + 0x14));
  if ((*((s32 *) (((s8 *) temp_a0) + 0))) != 1)
  {
    if (new_var)
    {
      new_var = temp_a0;
      return 0;
    }
    else
    {
      return 0;
    }
  }
  return (*((s32 *) (((s8 *) temp_a0) + 0x48))) & 0x4000;
}
#endif
