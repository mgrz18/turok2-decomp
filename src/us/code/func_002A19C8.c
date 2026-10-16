/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_4.c, func_80200C28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern s32 func_002A1384(s32 arg0);
extern void func_002A1350(s32 address, s32 value);
void func_002A19C8(s32 arg0, unsigned char arg1)
{
  s32 temp_s0;
  s32 temp_s2;
  temp_s2 = ((~arg0) & 3) * 8;
  arg0 &= ~3;
  func_002A1350(arg0, (func_002A1384(arg0) & (~(0xFF << temp_s2))) | ((arg1 & 0xFF) << temp_s2));
}
