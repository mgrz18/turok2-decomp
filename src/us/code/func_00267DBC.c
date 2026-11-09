#ifdef TARGET_PORT
/* The host: the same through CInstanceHeader/CIntelligence (func_00267D94's twin, bit 2); the
 * word view below reads the intelligence pointer at its N64 offset and size. */
#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

s32 func_00267DBC(CInstanceHeader *arg0) {
    CIntelligence *temp_a0 = arg0->m_pIntelligence;

    if (temp_a0->m_Type != 1) {
        return 0;
    }
    return temp_a0->m_dwTypeFlags & 2;
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
s32 func_00267DBC(void *arg0)
{
  void *temp_a0;
  unsigned char new_var;
  temp_a0 = *((void **) (((s8 *) arg0) + 0x14));
  new_var = 0;
  if ((*((s32 *) (((s8 *) temp_a0) + new_var))) != 1)
  {
    if (temp_a0)
    {
      return new_var;
    }
    else
    {
      return 0;
    }
  }
  return (*((s32 *) (((s8 *) temp_a0) + 0x14))) & 2;
}
#endif
