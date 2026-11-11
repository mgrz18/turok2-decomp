#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"

/* host body (round 8 w2): the mode-table animation picker of the gib deaths (modes 0x35/0x36):
 * the definition's gib model, + 1 in mode 0x36. The N64 text below reads the definition at the
 * N64 0x14 of the instance. */
volatile unsigned int EnemyAnim_ExtremeDeath(CGameObjectInstance *arg0, CAI *arg1) {
    s16 gib = arg0->header.m_pIntelligence->m_Enemy.m_GibModel;

    if (arg1->m_ModeBefore != 0x35) {
        return gib;
    }
    return gib + 1;
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
volatile unsigned int EnemyAnim_ExtremeDeath(void *arg0, void *arg1)
{
  if ((*((s8 *) (((s8 *) arg1) + 0x36))) != 0x35)
  {
    if (arg1)
    {
      return *((s16 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0x14)))) + 0x6A));
    }
    else
    {
      return *((s16 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0x14)))) + 0x6A));
    }
  }
  return (*((s16 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0x14)))) + 0x6A))) - -1;
}
#endif
