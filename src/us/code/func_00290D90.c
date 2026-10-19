/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/alCents2Ratio.c, alCents2Ratio.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
f32 func_00290D90(s32 cents) {
    f32 ratio = 1.0f; f32 base = 1.0005778f; if (cents < 0) { base = 0.99942254f; cents = -cents; } while (cents != 0) { if (cents & 1) { ratio *= base; } cents >>= 1; base *= base; } return ratio;
}
