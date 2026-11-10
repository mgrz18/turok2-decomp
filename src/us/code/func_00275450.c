#include "common.h"

s32 alCSeqNew(s32, s32);
s32 alCSPSetChlFXMix(s32, s32, s32);

s32 alCSPSetBank(sptr, s32);                    /* extern */
s32 alCSPSetSeq(sptr, s32);                    /* extern */
s32 alCSPSetVol(sptr, s16);                    /* extern */

void SndSeq_Load(void *arg0)
{
  f32 temp_f20;
  s32 temp_a1;
  s32 var_s0;
  var_s0 = 0;
  alCSeqNew(*((s32 *) (((s8 *) arg0) + 0x10)), *((s32 *) (((s8 *) arg0) + 0xC)));
  alCSPSetSeq(*((s32 *) (((s8 *) arg0) + 0x14)), *((s32 *) (((s8 *) arg0) + 0x10)));
  alCSPSetBank(*((s32 *) (((s8 *) arg0) + 0x14)), *((s32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 8)))) + 4)));
  do
  {
    temp_a1 = var_s0 & 0xFF;
    alCSPSetChlFXMix(*((s32 *) (((s8 *) arg0) + 0x14)), temp_a1, 0);
    var_s0 += 1;
  }
  while (var_s0 < 0x14);
  temp_f20 = ((f32) (*((s32 *) (((s8 *) arg0) + 0x24)))) * ((*((f32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0)))) + 0x2BA4))) * 0.01f);
  alCSPSetVol(*((s32 *) (((s8 *) arg0) + 0x14)), (s16) ((s32) (temp_f20 * 32767.0f)));
  *((f32 *) (((s8 *) arg0) + 0x2C)) = temp_f20;
}
