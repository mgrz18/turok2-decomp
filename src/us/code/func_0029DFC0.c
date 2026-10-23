/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/802591C0.CPP, func_80258D10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* NOTE: the Rage Wars match was a generic getter; Turok 2 has the libultra-style
 * CP0 Count read here (osGetCount). */

typedef unsigned int u32;

u32 func_0029DFC0(void) {
    u32 count;
    __asm__ __volatile__("mfc0 %0, $9" : "=r"(count)); /* CP0 $9 = Count */
    return count;
}
