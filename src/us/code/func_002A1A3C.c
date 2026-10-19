/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_80200D18_de.c, func_80200D18_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002A1954(s32 address, s32 value);
extern void func_002A1954(s32 address, s32 value);
extern void func_002A1350(s32 address, s32 value);
void func_002A1A3C(s32 address, const u8 *source, u32 length)
{
  u32 word;
  while ((length != 0) && ((address & 3) != 0))
  {
    func_002A1954(address++, *(source++));
 do { } while (0);
    length--;
  }
  if (length >= 4)
  {
    do
    {
      word = (((((u32) source[0]) << 24) | (((u32) source[1]) << 16)) | (((u32) source[2]) << 8)) | source[3];
      func_002A1350(address, word);
      address += 4;
      source += 4;
      length -= 4;
    }
    while (length >= 4);
  }
  while (length != 0)
  {
    func_002A1954(address++, *(source++));
    length--;
  }
}
