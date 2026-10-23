/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802609CC.CPP, func_80260A5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct Func802608ECResult;
typedef struct Func802608ECResult Func802608ECResult;
struct Func802608ECResult;
struct Func802608ECResult {
    u32 value;
    f32 start;
    f32 delta;
};
typedef struct { u32 low; u32 high; } PackedBitWords;
extern void func_00217EFC(s32 arg0, Func802608ECResult range, f32 arg4);
static __inline__ void write_bits(u32 arg2, u32 arg0, u32 arg1) {
  u32 temp_a0;
  s32 temp_v0;
  s32 temp_v0_2;
  u32 temp_a1;
  u32 temp_a2;
  u32 temp_shift;
  u32 var_a2;
  u32 var_a3;
  u32 var_v1;
  PackedBitWords *words;
  temp_a1 = (1 << arg1) - 1;
  var_v1 = temp_a1;
  temp_v0 = arg0 & 0x1F;
  temp_shift = temp_v0;
  temp_a2 = arg2 & temp_a1;
  if (temp_v0 == 0)
  {
    var_a3 = temp_a2;
    temp_a1 = 0;
    var_a2 = 0;
  }
  else
  {
    var_v1 = temp_a1 << temp_shift;
    temp_v0_2 = 0x20 - temp_shift;
    temp_a1 >>= temp_v0_2;
    var_a3 = temp_a2 << temp_shift;
    var_a2 = temp_a2 >> temp_v0_2;
  }
  temp_a0 = (arg0 & 0xF0000000) | (((u32) (arg0 & 0x0FFFFFE0)) >> 3);
  words = (PackedBitWords *) temp_a0;
  words->low = (words->low & (~var_v1)) | var_a3;
  words->high = (words->high & (~temp_a1)) | var_a2;
}
void func_00217EFC(s32 arg0, Func802608ECResult range, f32 arg4)
{
  f32 arg4_2;
  f32 temp_f1;
  s32 temp_f2;
  s32 var_a0;
  arg4_2 = arg4;
  temp_f1 = (arg4_2 - range.start) / range.delta * (f32) ((1 << range.value) - 1) + 0.5f;
  if (temp_f1 >= 0.0f)
  {
    var_a0 = (s32) temp_f1;
  }
  else
  {
    temp_f2 = (s32) temp_f1;
    var_a0 = 1;
    if ((f32) temp_f2 == temp_f1)
    {
      var_a0 = 0;
    }
    if (range.value)
    {
      var_a0 = temp_f2 - var_a0;
    }
    else
    {
      var_a0 = temp_f2 - var_a0;
    }
  }
  write_bits(var_a0, arg0, range.value);
}
