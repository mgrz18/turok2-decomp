#include "rw/junk_types.h" /* ALSeq */

s32 __alSeqReadVarLen();
s32 __alSeqNextDelta(ALSeq *seq, s32 *arg1)
{
  u32 temp_s1;
  temp_s1 = (u32) seq->curPtr;
  if (temp_s1 < ((u32) ((s32) seq->base + (s32) seq->len)))
  {
    if (seq)
    {
      *arg1 = __alSeqReadVarLen();
      seq->curPtr = (u8 *) temp_s1;
      return 1;
    }
    else
    {
      *arg1 = __alSeqReadVarLen();
      seq->curPtr = (u8 *) temp_s1;
      return 1;
    }
  }
  if (seq || temp_s1)
  {
    return 0;
  }
  else
  {
    return 0;
  }
}
