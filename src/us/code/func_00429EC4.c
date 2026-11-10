#ifdef TARGET_PORT
/* libultra's __osPfsSelectBank on the typed OSPfs (the m2c view below reads queue/channel as s32 at
 * N64 offsets and passes the buffer as an s32) */
#include "ultra_types.h"
extern s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
s32 __osPfsSelectBank(OSPfs *pfs) /* s32: libultra returns __osContRamWrite's result (abicheck void_used) */
{
    u8 temp[32];
    int i;
    for (i = 0; i < 32; i++) {
        temp[i] = pfs->activebank;
    }
    return __osContRamWrite(pfs->queue, pfs->channel, 0x8000 / 32, temp, 0);
}
#else
typedef unsigned char u8;
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

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x65 - 0xC];
    /* 0x65 */ u8 fill;
} Arg0;

s32 __osContRamWrite(sptr, s32, s32, sptr, s32);

/* Writes 0x20 bytes of the fill value at offset 0x400. */
void __osPfsSelectBank(Arg0 *arg0)
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
    *var_v1 = arg0->fill;
  }
  while (var_a0 < 0x20);
  __osContRamWrite(arg0->unk4, arg0->unk8, 0x400, (s32) sp18, 0);
}
#endif
