
#ifndef TARGET_PORT
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
#endif
#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"
#define T2_HOST_FLAGS_OFFSET __builtin_offsetof(CGameObjectInstance, m_dwFlags)
#endif
s32 func_00268BE0(void *arg0)
{
  u8 *new_var;
  int new_var2;
  new_var2 = 0;
  new_var = &(*((u8 *) (((s8 *) arg0) + 0)));
  if ((*new_var) != 1)
  {
    if (arg0)
    {
      return new_var2;
    }
    else
      if (arg0)
    {
      return new_var2;
    }
    else
    {
      return new_var2;
    }
  }
#ifdef TARGET_PORT /* m_dwFlags at its host offset (round 11 C26: the N64 pads put the flags (0xD4) on host m_rpGeometryHeader/m_rpObjectHeader) */
  return (*((s32 *) (((s8 *) arg0) + T2_HOST_FLAGS_OFFSET))) & 1;
#else
  return (*((s32 *) (((s8 *) arg0) + 0xD4))) & 1;
#endif
}
