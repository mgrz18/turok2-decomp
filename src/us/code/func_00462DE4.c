#ifdef TARGET_PORT
/* libultra's __osPfsSelectBank on the typed OSPfs (the m2c view below reads queue/channel as s32 at
 * N64 offsets and passes the buffer as an s32) */
#include "ultra_types.h"
extern s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
void __osPfsSelectBank_2(OSPfs *pfs)
{
    u8 temp[32];
    int i;
    for (i = 0; i < 32; i++) {
        temp[i] = pfs->activebank;
    }
    __osContRamWrite(pfs->queue, pfs->channel, 0x8000 / 32, temp, 0);
}
#else

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
volatile int __osContRamWrite(sptr, s32, s32, sptr, s32);
void __osPfsSelectBank_2(void *arg0)
{
  u8 sp18[0x20];
  s32 var_a0;
  u8 *var_v1;
  var_a0 = 0;
  var_v1 = sp18;
  do
  {
    var_v1 = &sp18[var_a0];
    var_a0 += 1;
    *var_v1 = *((u8 *) (((s8 *) arg0) + 0x65));
  }
  while (var_a0 < 0x20);
  var_a0 = *((s32 *) (((s8 *) arg0) + 4));
  __osContRamWrite(*((s32 *) (((s8 *) arg0) + 4)), *((s32 *) (((s8 *) arg0) + 8)), 0x400, (s32) sp18, 0);
}
#endif
