/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804422F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
extern float func_004162FC(int c, unsigned char next, float sx, float sy);
f32 func_004162FC(s32 arg0, u8 arg1, f32 arg2, f32 arg3);
f32 func_004163C0(u8 *p, f32 sx, f32 sy) {
    f32 total;
    u8 c;
    total = 0.0f;
    while (p != 0 && (c = *p) != 0 && c != '\n') {
        p++;
        total += func_004162FC(c, *p, sx, sy);
    }
    return total;
}
