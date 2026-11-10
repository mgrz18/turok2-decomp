#include "rw/junk_types.h" /* ALSeq */

s32 __alSeqReadVarLen();
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS __alSeqReadVarLen receives __alSeqNextDelta's own argument
 * registers (pass-through); the host passes them. */
s32 __alSeqNextDelta(ALSeq *seq, s32 *arg1)
{
  uptr temp_s1;
  temp_s1 = (uptr) seq->curPtr;
  if (temp_s1 < ((u32) ((sptr) seq->base + (s32) seq->len)))
  {
    if (seq)
    {
      *arg1 = __alSeqReadVarLen(seq);
      seq->curPtr = (u8 *) temp_s1;
      return 1;
    }
    else
    {
      *arg1 = __alSeqReadVarLen(seq);
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
#else
s32 __alSeqNextDelta(ALSeq *seq, s32 *arg1)
{
  uptr temp_s1;
  temp_s1 = (uptr) seq->curPtr;
  if (temp_s1 < ((u32) ((sptr) seq->base + (s32) seq->len)))
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
#endif
