/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/802591C0.CPP, func_80258D10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Writes the CP0 Compare register (__osSetCompare). */
void func_0029E210(unsigned int value) {
    __asm__ __volatile__("mtc0 %0, $11" : : "r"(value));
}
