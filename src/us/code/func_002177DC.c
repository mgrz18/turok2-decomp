/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/8025E5B0_8.CPP, func_8026033C_de.
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
extern float D_800A59C8;
typedef struct { u32 low; u32 high; } PackedBitWords;
static __inline__ void write_bits(u32 value, u32 bitAddress, u32 width) {
  u32 address;
  s32 shift;
  s32 remaining;
  u32 highMask;
  u32 offset;
  u32 high;
  u32 low;
  u32 lowMask;
  PackedBitWords *words;
  highMask = (1 << width) - 1;
  lowMask = highMask;
  shift = bitAddress & 0x1F;
  offset = shift;
  value &= highMask;
  if (shift == 0)
  {
    low = value;
    highMask = 0;
    high = 0;
  }
  else
  {
    lowMask = highMask << offset;
    remaining = 0x20 - offset;
    highMask >>= remaining;
    low = value << offset;
    high = value >> remaining;
  }
  address = (bitAddress & 0xF0000000) | (((u32) (bitAddress & 0x0FFFFFE0)) >> 3);
  words = (PackedBitWords *) address;
  words->low = (words->low & (~lowMask)) | low;
  words->high = (words->high & (~highMask)) | high;
}
static __inline__ void writeQuantized(u32 bitAddress, Func802608ECResult range, f32 value) {
    f32 scaled;
    s32 integral;
    s32 quantized;
    scaled = (((value - range.start) / range.delta) * (f32) ((1 << range.value) - 1)) + D_800A59C8;
    if (scaled >= 0.0f) {
        quantized = (s32) scaled;
    } else {
        integral = (s32) scaled;
        quantized = 1;
        if ((f32) integral == scaled) {
            quantized = 0;
        }
        quantized = integral - quantized;
    }
    write_bits(quantized, bitAddress, range.value);
}
void func_002177DC(u32 *stream, Func802608ECResult range, f32 value) {
    writeQuantized(*stream, range, value);
    *stream += range.value;
}
