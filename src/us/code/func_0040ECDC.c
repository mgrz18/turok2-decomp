#include "common.h"

/* A 0x64-byte slot with a swoosh trail at 0x3C (see func_0040EEA0, which
 * reaches the same block as D_80130200). */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ u8 trail[0x64 - 0x3C];
} TrailSlot;

typedef struct {
    /* 0x000 */ TrailSlot slots[12];
    /* 0x4B0 */ u8 pad4B0[0x4EC - 0x4B0];
    /* 0x4EC */ s32 slotCount;
    /* 0x4F0 */ u8 pad4F0[0x4FC - 0x4F0];
    /* 0x4FC */ s32 mode;
    /* 0x500 */ u8 pad500[0x518 - 0x500];
    /* 0x518 */ u8 sub0[0x18];
    /* 0x530 */ u8 sub1[0x18];
    /* 0x548 */ u8 sub2[0x18];
} TrailSet;

/* Per-mode handlers, 8 bytes each. */
typedef struct {
    /* 0x0 */ void (*fn)(void);
    /* 0x4 */ s32 unk4;
} ModeHandler;

s32 func_00235510(s32);
s32 func_0040C850(s32);
s32 func_0040CB28(s32);
s32 func_0027AD88(s32 *, s32);
extern ModeHandler D_00432410[];
extern s32 D_801100F0;

void func_0040ECDC(TrailSet *arg0)
{
  void (*temp_v0)(void);
  s32 var_s0;
  s32 var_s1;
  temp_v0 = D_00432410[arg0->mode].fn;
  if (temp_v0 != 0)
  {
    temp_v0();
  }
  var_s0 = 0;
  if (arg0->slotCount > 0)
  {
    var_s1 = 0x3C;      /* slots[i].trail */
    do
    {
      func_00235510((s32) ((u8 *) arg0 + var_s1));
      var_s0 += 1;
      var_s1 += sizeof(TrailSlot);
      var_s0++;
      var_s0--;
    }
    while (var_s0 < arg0->slotCount);
  }
  func_0040C850((s32) arg0->sub0);
  func_0040C850((s32) arg0->sub1);
  func_0040C850((s32) arg0->sub2);
  func_0027AD88(&D_801100F0, func_0040CB28((s32) arg0));
}
