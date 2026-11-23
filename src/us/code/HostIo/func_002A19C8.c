/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_4.c, func_80200C28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 HostIo_ReadWord(s32 arg0);
extern void HostIo_WriteWord(s32 address, s32 value);
void HostIo_WriteByteDma(s32 arg0, unsigned char arg1)
{
  s32 temp_s0;
  s32 temp_s2;
  temp_s2 = ((~arg0) & 3) * 8;
  arg0 &= ~3;
  HostIo_WriteWord(arg0, (HostIo_ReadWord(arg0) & (~(0xFF << temp_s2))) | ((arg1 & 0xFF) << temp_s2));
}
