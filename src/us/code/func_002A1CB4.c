/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_6.c, func_80200F98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern void func_002A1954(s32 address, s32 value);
extern void func_002A1350(s32 address, s32 value);
void func_002A1CB4(s32 address, s32 value, u32 count)
{
  u32 new_var;
  u32 byte;
  unsigned char new_var2;
  u32 word;
  new_var2 = value & 0xFF;
  while ((count != 0) && ((address & 3) != 0))
  {
    func_002A1954(address++, new_var2);
    count--;
  }
  if (count >= 4)
  {
    byte = new_var2;
    new_var = byte;
    word = (((byte << 24) | (byte << 16)) | (new_var << 8)) | new_var;
    do
    {
      func_002A1350(address, word);
      count -= 4;
      address += 4;
    }
    while (count >= 4);
  }
  while (count)
  {
    func_002A1954(address++, new_var2);
    count--;
  }
}
