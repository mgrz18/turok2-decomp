/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/802591C0.CPP, func_80258D10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* sqrtf: a single sqrt.s (the compiler has no sqrtf builtin). */
float func_00298470(float x) {
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}
