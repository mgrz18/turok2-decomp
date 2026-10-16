/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de.c, func_80260B80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
struct Func802608ECResult;
typedef struct Func802608ECResult Func802608ECResult;
struct Func802608ECResult;
struct Func802608ECResult {
    u32 value;
    f32 start;
    f32 delta;
};
struct LocalDecodeRange;
struct LocalDecodeRange {
    u32 unused;
    Func802608ECResult range;
};
struct LocalDecodeRange;
typedef struct LocalDecodeRange LocalDecodeRange;
extern double D_800A59E0;
f32 func_00218020(u32 *stream, Func802608ECResult range) {
    u32 bitAddress;
    u32 *streamPtr;
    s32 shift;
    u32 *word;
    u32 value;
    u32 next;
    u32 width;
    f64 converted;
    f32 result;
    LocalDecodeRange local;
    streamPtr = stream;
    bitAddress = *streamPtr;
    local.range = range;
    width = local.range.value;
    word = (u32 *) ((bitAddress & 0xF0000000) |
                    ((bitAddress & 0x0FFFFFE0) >> 3));
    shift = bitAddress & 0x1F;
    value = word[0];
    next = word[1];
    if (shift != 0) {
        value >>= shift;
        next <<= 0x20 - shift;
        value |= next;
    }
    if (width < 0x20U) {
        value &= (1 << width) - 1;
    }
    converted = (s32) value;
    if ((s32) value < 0) {
        converted += D_800A59E0;
    }
    result = (f32) converted;
    result /= (f32) ((1 << width) - 1);
    result *= local.range.delta;
    result += local.range.start;
    *streamPtr += range.value;
    return result;
}
