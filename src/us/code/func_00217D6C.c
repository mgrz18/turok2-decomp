/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_802608CC_de.
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
extern Func802608ECResult func_00217D6C(f32 arg0, f32 arg1, f32 arg2);
Func802608ECResult func_00217D6C(f32 arg0, f32 arg1, f32 arg2) {
    Func802608ECResult result;
    f32 ratio;
    s32 truncated;
    s32 bits;
    u32 count;
    u32 highest;
    u32 bit;
    u32 value;
    result.start = arg0;
    result.delta = arg1 - arg0;
    ratio = result.delta / (arg2 + arg2);
    if (ratio <= 0.0f) {
        bits = (s32)ratio + 1;
    } else {
        truncated = (s32)ratio;
        bits = truncated + 1;
        if ((f32)truncated != ratio) {
            bits = truncated + 2;
        }
    }
    count = 0;
    highest = 0;
    bit = 0;
    do {
        if (bits & (u32)(1 << bit)) {
            count += 1;
            highest = bit;
        }
        bit += 1;
    } while (bit < 0x20U);
    value = highest;
    if (count >= 2U) {
        value += 1;
    }
    result.value = value;
    return result;
}
