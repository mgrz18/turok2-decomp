#include "common.h"

s32 CCache__Compact(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

/* Two counters 0x3C past the message queue at gCache_mutex. */
typedef struct {
    /* 0x0 */ s32 pending;
    /* 0x4 */ s32 count;
} Counters;

extern u8 gCache_mutex[];
extern s32 gCache_mutex_count;
extern Counters gCache_frameDelay;

void CCache__Advance(void)
{
  s32 temp_a0;
  s8 *new_var;
  s32 temp_v0;
  s32 temp_v1;
  s32 temp_v1_2;
  s32 var_s0;
  if (gCache_frameDelay.pending != 0)
  {
    new_var = (s8 *) (&gCache_frameDelay);
    gCache_frameDelay.pending = gCache_frameDelay.pending - 1;
  }
  gCache_frameDelay.count = gCache_frameDelay.count + 1;
  temp_a0 = __osDisableInt();
  temp_v1 = gCache_mutex_count + 1;
  gCache_mutex_count = temp_v1;
  if (temp_v1 != 1)
  {
    __osRestoreInt(temp_a0);
    osRecvMesg((s32) (((s8 *) (&gCache_frameDelay)) - 0x3C), 0, 1);
    var_s0 = 0;
  }
  else
  {
    __osRestoreInt(temp_a0);
    var_s0 = 0;
  }
  do
  {
    CCache__Compact(0, 0);
    var_s0 += 1;
  }
  while (var_s0 < 0xA);
  temp_v0 = __osDisableInt();
  temp_v1_2 = gCache_mutex_count - 1;
  gCache_mutex_count = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    __osRestoreInt(temp_v0);
    osSendMesg((s32) gCache_mutex, 0, 1);
    return;
  }
  __osRestoreInt(temp_v0);
}
